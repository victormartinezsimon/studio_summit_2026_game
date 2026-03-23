#pragma once

#include <chrono>
#include <cstdio>
#include <array>
#include <cstring>

#ifdef PROFILING_ENABLED

class Profiler
{
public:
    static constexpr int MAX_SECTIONS = 16;
    static constexpr int REPORT_INTERVAL_FRAMES = 120;

    struct Section
    {
        const char* name;
        double totalUs;
        double minUs;
        double maxUs;
        int count;
    };

    static Profiler& Instance()
    {
        static Profiler instance;
        return instance;
    }

    void BeginFrame()
    {
        _frameStart = Clock::now();
    }

    void EndFrame()
    {
        auto now = Clock::now();
        double frameUs = ToMicroseconds(_frameStart, now);
        _frameTotalUs += frameUs;

        if (frameUs < _frameMinUs) _frameMinUs = frameUs;
        if (frameUs > _frameMaxUs) _frameMaxUs = frameUs;

        ++_frameCount;

        if (_frameCount >= REPORT_INTERVAL_FRAMES)
        {
            PrintReport();
            Reset();
        }
    }

    void BeginSection(int id, const char* name)
    {
        if (id >= MAX_SECTIONS) return;
        _sections[id].name = name;
        _sectionStarts[id] = Clock::now();
    }

    void EndSection(int id)
    {
        if (id >= MAX_SECTIONS) return;
        auto now = Clock::now();
        double us = ToMicroseconds(_sectionStarts[id], now);
        auto& s = _sections[id];
        s.totalUs += us;
        if (us < s.minUs) s.minUs = us;
        if (us > s.maxUs) s.maxUs = us;
        ++s.count;
    }

private:
    using Clock = std::chrono::steady_clock;
    using TimePoint = Clock::time_point;

    Profiler() { Reset(); }

    double ToMicroseconds(TimePoint start, TimePoint end) const
    {
        return std::chrono::duration<double, std::micro>(end - start).count();
    }

    void PrintReport()
    {
        double avgFrame = _frameTotalUs / _frameCount;

        std::printf("\n===== PROFILE REPORT (%d frames) =====\n", _frameCount);
        std::printf("Frame:  avg=%.1f us  min=%.1f us  max=%.1f us  (avg=%.2f ms)\n",
                    avgFrame, _frameMinUs, _frameMaxUs, avgFrame / 1000.0);

        for (int i = 0; i < MAX_SECTIONS; ++i)
        {
            auto& s = _sections[i];
            if (s.count == 0) continue;
            double avg = s.totalUs / s.count;
            double pct = (s.totalUs / _frameTotalUs) * 100.0;
            std::printf("  [%s]  avg=%.1f us  min=%.1f us  max=%.1f us  (%.1f%% of frame)\n",
                        s.name, avg, s.minUs, s.maxUs, pct);
        }

        std::printf("======================================\n\n");
        std::fflush(stdout);
    }

    void Reset()
    {
        _frameCount = 0;
        _frameTotalUs = 0.0;
        _frameMinUs = 1e12;
        _frameMaxUs = 0.0;
        for (auto& s : _sections)
        {
            s.name = "";
            s.totalUs = 0.0;
            s.minUs = 1e12;
            s.maxUs = 0.0;
            s.count = 0;
        }
    }

    TimePoint _frameStart;
    TimePoint _sectionStarts[MAX_SECTIONS];
    Section _sections[MAX_SECTIONS];
    int _frameCount = 0;
    double _frameTotalUs = 0.0;
    double _frameMinUs = 1e12;
    double _frameMaxUs = 0.0;
};

// Convenience macros
#define PROFILE_BEGIN_FRAME()       Profiler::Instance().BeginFrame()
#define PROFILE_END_FRAME()         Profiler::Instance().EndFrame()
#define PROFILE_BEGIN(id, name)     Profiler::Instance().BeginSection(id, name)
#define PROFILE_END(id)             Profiler::Instance().EndSection(id)

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

#endif
