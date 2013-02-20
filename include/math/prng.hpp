#pragma once

#include <common/common.hpp>
#include <engine/architecture.hpp>

/** @file prng.hpp
  * @brief Lightweight device PRNG.
**/

/** @class PRNG
  * @brief Pseudorandom number generator.
  *
  * This is a lightweight, cryptographic-grade pseudorandom number generator,
  * wrapped up as a kernel object. It is active, as it uploads a new seed to
  * the device at every kernel invocation.
**/
class PRNG : public KernelObject
{
    private:
        /** @brief We use a 64-bit seed though we could go up to 256 bits. **/
        uint64_t seed;

        double progress;

        /** @brief This is the device-side buffer containing the seed. **/
        cl::Buffer buffer;
    public:
        bool IsActive();
        void Initialize(const EngineParams& params);
        void Bind(cl::Kernel kernel, cl_uint slot);
        void Update(const EngineParams& params, size_t index);
        void* Query(const EngineParams& params, size_t query);
        void Cleanup(const EngineParams& params);
};