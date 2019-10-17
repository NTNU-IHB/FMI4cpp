/*
 * The MIT License
 *
 * Copyright 2017-2018 Norwegian University of Technology
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING  FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef FMI4CPP_ABSTRACTFMUINSTANCE_HPP
#define FMI4CPP_ABSTRACTFMUINSTANCE_HPP

#include <fmi4cpp/fmu_instance.hpp>
#include <fmi4cpp/fmu_resource.hpp>
#include <fmi4cpp/types.hpp>

#include <memory>
#include <string>

namespace fmi4cpp
{

template<typename fmi_library, typename model_description>
class fmu_instance_base : public virtual fmu_instance<model_description>
{

private:
    bool terminated_ = false;
    bool instanceFreed_ = false;
    std::shared_ptr<fmu_resource> resource_;

protected:
    fmi4cppComponent c_;
    const std::shared_ptr<fmi_library> library_;
    const std::shared_ptr<const model_description> modelDescription_;

public:
    fmu_instance_base(fmi4cppComponent c,
        std::shared_ptr<fmu_resource> resource,
        const std::shared_ptr<fmi_library>& library,
        const std::shared_ptr<const model_description>& modelDescription)
        : c_(c)
        , resource_(std::move(resource))
        , library_(library)
        , modelDescription_(modelDescription)
    {}

    std::shared_ptr<const model_description> get_model_description() const override
    {
        return modelDescription_;
    }

    bool set_debug_logging(const bool loggingOn, const std::vector<const char*> categories) const
    {
        return library_->set_debug_logging(c_, loggingOn, categories);
    }

    bool setup_experiment(double start = 0, double stop = 0, double tolerance = 0) override
    {
        this->simulationTime_ = start;
        return library_->setup_experiment(c_, tolerance, start, stop);
    }

    bool enter_initialization_mode() override
    {
        return library_->enter_initialization_mode(c_);
    }

    bool exit_initialization_mode() override
    {
        return library_->exit_initialization_mode(c_);
    }

    bool reset() override
    {
        return library_->reset(c_);
    }

    bool terminate() override
    {
        return terminate(true);
    }

    bool terminate(bool freeInstance)
    {
        if (!this->terminated_) {
            this->terminated_ = true;
            if (!library_->terminate(c_)) {
                return false;
            }
            this->free_instance();
        }
        return true;
    }

    void free_instance()
    {
        if (!instanceFreed_) {
            instanceFreed_ = true;
            library_->free_instance(c_);
            c_ = nullptr;
        }
    }

    bool get_fmu_state(fmi4cppFMUstate& state) override
    {
        return library_->get_fmu_state(c_, state);
    }

    bool set_fmu_state(fmi4cppFMUstate state) override
    {
        return library_->set_fmu_state(c_, state);
    }

    bool free_fmu_state(fmi4cppFMUstate& state) override
    {
        return library_->free_fmu_state(c_, state);
    }


    bool get_serialized_fmu_state_size(fmi4cppFMUstate state, size_t& size) const
    {
        return library_->get_serialized_fmu_state_size(c_, state, size);
    }

    bool serialize_fmu_state(const fmi4cppFMUstate& state, std::vector<fmi4cppByte>& serializedState) override
    {
        return library_->serialize_fmu_state(c_, state, serializedState);
    }

    bool de_serialize_fmu_state(fmi4cppFMUstate& state, const std::vector<fmi4cppByte>& serializedState) override
    {
        return library_->de_serialize_fmu_state(c_, state, serializedState);
    }

    bool get_directional_derivative(
        const std::vector<fmi4cppValueReference>& vUnknownRef,
        const std::vector<fmi4cppValueReference>& vKnownRef,
        const std::vector<fmi4cppReal>& dvKnownRef, std::vector<fmi4cppReal>& dvUnknownRef) override
    {
        return library_->get_directional_derivative(c_, vUnknownRef, vKnownRef, dvKnownRef, dvUnknownRef);
    }

    bool read_integer(const fmi4cppValueReference vr, fmi4cppInteger& ref) override
    {
        return library_->read_integer(c_, vr, ref);
    }

    bool read_integer(const std::vector<fmi4cppValueReference>& vr, std::vector<fmi4cppInteger>& ref) override
    {
        return library_->read_integer(c_, vr, ref);
    }

    bool read_real(const fmi4cppValueReference vr, fmi4cppReal& ref) override
    {
        return library_->read_real(c_, vr, ref);
    }

    bool read_real(const std::vector<fmi4cppValueReference>& vr, std::vector<fmi4cppReal>& ref) override
    {
        return library_->read_real(c_, vr, ref);
    }

    bool read_string(const fmi4cppValueReference vr, fmi4cppString& ref) override
    {
        return library_->read_string(c_, vr, ref);
    }

    bool read_string(const std::vector<fmi4cppValueReference>& vr, std::vector<fmi4cppString>& ref) override
    {
        return library_->read_string(c_, vr, ref);
    }

    bool read_boolean(const fmi4cppValueReference vr, fmi4cppBoolean& ref) override
    {
        return library_->read_boolean(c_, vr, ref);
    }

    bool read_boolean(const std::vector<fmi4cppValueReference>& vr, std::vector<fmi4cppBoolean>& ref) override
    {
        return library_->read_boolean(c_, vr, ref);
    }

    bool write_integer(const fmi4cppValueReference vr, const fmi4cppInteger value) override
    {
        return library_->write_integer(c_, vr, value);
    }

    bool write_integer(const std::vector<fmi4cppValueReference>& vr, const std::vector<fmi4cppInteger>& values) override
    {
        return library_->write_integer(c_, vr, values);
    }

    bool write_real(const fmi4cppValueReference vr, const fmi4cppReal value) override
    {
        return library_->write_real(c_, vr, value);
    }

    bool write_real(const std::vector<fmi4cppValueReference>& vr, const std::vector<fmi4cppReal>& values) override
    {
        return library_->write_real(c_, vr, values);
    }

    bool write_string(const fmi4cppValueReference vr, fmi4cppString value) override
    {
        return library_->write_string(c_, vr, value);
    }

    bool write_string(const std::vector<fmi4cppValueReference>& vr, const std::vector<fmi4cppString>& values) override
    {
        return library_->write_string(c_, vr, values);
    }

    bool write_boolean(const fmi4cppValueReference vr, const fmi4cppBoolean value) override
    {
        return library_->write_boolean(c_, vr, value);
    }

    bool write_boolean(const std::vector<fmi4cppValueReference>& vr, const std::vector<fmi4cppBoolean>& values) override
    {
        return library_->write_boolean(c_, vr, values);
    }

    ~fmu_instance_base()
    {
        terminate();
    }
};

} // namespace fmi4cpp

#endif //FMI4CPP_ABSTRACTFMUINSTANCE_HPP
