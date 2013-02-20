#include <math/prng.hpp>

/* Device-side representation. */
struct cl_prng { cl_ulong4 seed; };

bool PRNG::IsActive() { return true; }

void PRNG::Initialize(const EngineParams& params)
{
    cl_int error;
    this->buffer = cl::Buffer(params.context, CL_MEM_READ_ONLY,
                              sizeof(cl_prng), nullptr, &error);
    Error::Check(Error::Memory, error);

    this->seed = 0;
}

void PRNG::Bind(cl::Kernel kernel, cl_uint index)
{
    Error::Check(Error::Bind, kernel.setArg(index, this->buffer));
}

void PRNG::Update(const EngineParams& params, size_t index)
{
    cl_int error = params.queue.enqueueWriteBuffer(this->buffer, CL_FALSE, 0,
                                                   sizeof(uint64_t),
                                                   &this->seed);
    Error::Check(Error::CLIO, error);
    this->seed++;
}

void* PRNG::Query(const EngineParams& params, size_t query)
{
    if (query == Query::Progress)
    {
        /* Most kernel objects can return progress. */
        size_t total = params.samples - 1;
        progress = (double)this->seed / total;
        return &progress;
    }

    return nullptr;
}

void PRNG::Cleanup(const EngineParams& params)
{
    /* Don't need to do anything here, thanks to RAII. More complicated kernel
     * objects which use dynamic structures will need to do some cleanup. */
}