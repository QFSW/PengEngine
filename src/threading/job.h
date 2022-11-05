#pragma once

#include <functional>

namespace threading
{
    class Job
    {
    public:
        Job(std::function<void()> f);

        void execute() const;
        static Job empty();

    private:
        std::function<void()> _f;
    };
}