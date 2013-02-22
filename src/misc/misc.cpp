#include <misc/misc.hpp>

bool Progress::IsActive() { return true; }

void Progress::Initialize()
{
    return;
}

void Progress::Bind(cl_uint* slot)
{
    return;
}

void Progress::Update(size_t index)
{
    this->progress = (double)index / (params.samples - 1);

    if (index == 0) this->startTime = time(nullptr);

    this->elapsed = difftime(time(nullptr), this->startTime);
    if (this->elapsed < 5)
    {
        this->ETC = -1.0; /* Indeterminate. */
    }
    else
    {
        this->ETC = this->elapsed * (double)(params.samples - index) / index;
    }
}

void* Progress::Query(size_t query)
{
    if (query == Query::Progress) return &this->progress;
    if (query == Query::EstimatedTime) return &this->ETC;
    if (query == Query::ElapsedTime) return &this->elapsed;
    return nullptr;
}
