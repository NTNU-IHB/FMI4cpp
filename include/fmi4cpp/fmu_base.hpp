
#ifndef FMI4CPP_FMU_HPP
#define FMI4CPP_FMU_HPP

#include <memory>
#include <string>

namespace fmi4cpp
{

template<class ModelDescription>
class fmu_base
{

public:
    [[nodiscard]] std::string guid() const
    {
        return get_model_description()->guid;
    }

    [[nodiscard]] std::string model_name() const
    {
        return get_model_description()->model_name;
    }

    virtual std::shared_ptr<const ModelDescription> get_model_description() const = 0;

    virtual ~fmu_base() = default;
};

template<class model_description, class cs_fmu, class me_fmu>
class fmu_provider : public virtual fmu_base<model_description>
{

public:
    [[nodiscard]] virtual bool supports_cs() const = 0;
    [[nodiscard]] virtual bool supports_me() const = 0;

    virtual std::unique_ptr<cs_fmu> as_cs_fmu() const = 0;
    virtual std::unique_ptr<me_fmu> as_me_fmu() const = 0;
};

template<class cs_slave, class cs_model_description>
class cs_fmu_base : public fmu_base<cs_model_description>
{

public:
    virtual std::shared_ptr<const cs_model_description> get_model_description() const = 0;

    virtual std::unique_ptr<cs_slave> new_instance(bool visible, bool loggingOn) = 0;
};

template<class me_instance, class me_model_description>
class me_fmu_base : public fmu_base<me_model_description>
{

public:
    virtual std::shared_ptr<const me_model_description> get_model_description() const = 0;

    virtual std::unique_ptr<me_instance> new_instance(bool visible, bool loggingOn) = 0;
};

} // namespace fmi4cpp


#endif //FMI4CPP_FMU_HPP
