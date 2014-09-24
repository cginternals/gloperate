#pragma once

#include <vector>
#include <string>
#include <set>

#include <gloperate/gloperate_api.h>

#include <gloperate/pipelines/Nameable.h>

namespace gloperate
{

class AbstractData;
class AbstractStage;
class AbstractInputSlot;
class AbstractParameter;
template <typename T>
class Parameter;
template <typename T>
class ConvertibleParameter;
template <typename T>
class Data;

class GLOPERATE_API AbstractPipeline : public Nameable
{
public:
    AbstractPipeline();
    virtual ~AbstractPipeline();

    bool isInitialized() const;
    void initialize();

    virtual void execute();

    virtual void addStage(AbstractStage * stage);

    void addParameter(AbstractParameter * parameter);
    void addParameter(const std::string & name, AbstractParameter * parameter);

    void shareData(const AbstractData* data);
    void shareDataFrom(const AbstractInputSlot& slot);

    template <typename T>
    Parameter<T> * createParameter(const std::string & name, const T & value);

    std::vector<AbstractInputSlot*> allInputs();
    std::vector<AbstractData*> allOutputs();

    AbstractParameter * findParameter(const std::string & name);
    std::vector<AbstractData*> findOutputs(const std::string & name);

    template <typename T>
    Parameter<T> * getParameter(const std::string & name);
    template <typename T>
    Parameter<T> * getParameter();

    template <typename T>
    Data<T> * getOutput(const std::string & name);
    template <typename T>
    Data<T> * getOutput();

    template<typename T, typename... Args>
    void addStages(T stage, Args... pipeline);

    const std::vector<AbstractStage*> & stages() const;
    const std::vector<AbstractParameter*> & parameters() const;

    std::set<AbstractParameter*> unusedParameters();
protected: 
    void sortDependencies();
    void addStages();
    void initializeStages();

    void tsort(std::vector<AbstractStage*> & stages);
protected:
    bool m_initialized;
    std::vector<AbstractStage*> m_stages;
    std::vector<AbstractParameter*> m_parameters;
    std::vector<const AbstractData*> m_sharedData;
    bool m_dependenciesSorted;
private:
    AbstractPipeline(const AbstractPipeline&) = delete;
};

} // namespace gloperate

#include <gloperate/pipelines/AbstractPipeline.hpp>
