#pragma once

#include <chrono>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <vector>

#ifdef PROFILING_ENABLED

class Profiler
{
public:
    static constexpr int MAX_SECTIONS = 16;

    struct TraceEvent
    {
        const char* name;
        double ts;
        double dur;
    };

    static Profiler& Instance()
    {
        static Profiler instance;
        return instance;
    }

    ~Profiler()
    {
        SaveToFile();
    }

    void BeginFrame()
    {
        _frameStart = Clock::now();
    }

    void EndFrame()
    {
        auto now = Clock::now();
        double ts = ToMicroseconds(_profileStart, _frameStart);
        double dur = ToMicroseconds(_frameStart, now);
        _events.push_back({"Frame", ts, dur});
    }

    void BeginSection(int id, const char* name)
    {
        if (id >= MAX_SECTIONS) return;
        _sectionNames[id] = name;
        _sectionStarts[id] = Clock::now();
    }

    void EndSection(int id)
    {
        if (id >= MAX_SECTIONS) return;
        auto now = Clock::now();
        double ts = ToMicroseconds(_profileStart, _sectionStarts[id]);
        double dur = ToMicroseconds(_sectionStarts[id], now);
        _events.push_back({_sectionNames[id], ts, dur});
    }

    void SaveToFile()
    {
        if (_events.empty()) return;

        auto now = std::time(nullptr);
        char filename[64];
        std::strftime(filename, sizeof(filename), "profile_%Y%m%d_%H%M%S.json", std::localtime(&now));

        FILE* f = std::fopen(filename, "w");
        if (!f) return;

        std::fprintf(f, "{\"traceEvents\":[\n");
        for (size_t i = 0; i < _events.size(); ++i)
        {
            auto& e = _events[i];
            std::fprintf(f, "{\"name\":\"%s\",\"ph\":\"X\",\"ts\":%.1f,\"dur\":%.1f,\"pid\":0,\"tid\":0}",
                         e.name, e.ts, e.dur);
            if (i + 1 < _events.size()) std::fprintf(f, ",");
            std::fprintf(f, "\n");
        }
        std::fprintf(f, "]}\n");
        std::fclose(f);

        _events.clear();
    }

private:
    using Clock = std::chrono::steady_clock;
    using TimePoint = Clock::time_point;

    Profiler() : _profileStart(Clock::now())
    {
        _events.reserve(100000);
    }

    double ToMicroseconds(TimePoint start, TimePoint end) const
    {
        return std::chrono::duration<double, std::micro>(end - start).count();
    }

    TimePoint _profileStart;
    TimePoint _frameStart;
    TimePoint _sectionStarts[MAX_SECTIONS];
    const char* _sectionNames[MAX_SECTIONS] = {};
    std::vector<TraceEvent> _events;
};

// Convenience macros
#define PROFILE_BEGIN_FRAME()       Profiler::Instance().BeginFrame()
#define PROFILE_END_FRAME()         Profiler::Instance().EndFrame()
#define PROFILE_BEGIN(id, name)     Profiler::Instance().BeginSection(id, name)
#define PROFILE_END(id)             Profiler::Instance().EndSection(id)
#define PROFILE_SAVE()              Profiler::Instance().SaveToFile()

// RAII scoped section
struct ProfileScope
{
    int id;
    ProfileScope(int id, const char* name) : id(id) { PROFILE_BEGIN(id, name); }
    ~ProfileScope() { PROFILE_END(id); }
};
#define PROFILE_SCOPE(id, name) ProfileScope _profileScope##id(id, name)

#else

#define PROFILE_BEGIN_FRAME()
#define PROFILE_END_FRAME()
#define PROFILE_BEGIN(id, name)
#define PROFILE_END(id)
#define PROFILE_SCOPE(id, name)
#define PROFILE_SAVE()

#endif
