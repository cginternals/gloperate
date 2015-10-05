
#pragma once

#include <memory>
#include <set>
#include <string>
#include <vector>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


class AbstractData;
class AbstractStage;
class AbstractInputSlot;

template <typename T>
class Data;


/**
*  @brief
*    Rendering pipeline
*
*    A pipeline describes a rendering or computation process.
*    It contains a number of stages, which are executed in a
*    defined order and only when needed.
*
*    Rendering stages provide data containers and input
*    slots, which can be connected to each other. Using
*    these data objects, a stage announces which input and
*    output data it needs or provides. Using this, it can be
*    determined which stage has to be executed and in what order
*    the stages are processed.
*
*  @see AbstractStage
*  @see Data
*  @see InputSlot
*/
class GLOPERATE_API AbstractPipeline
{
public:
    AbstractPipeline(const std::string & name = "");
    virtual ~AbstractPipeline();

    // Fixes issues with MSVC2013 Update 3
    AbstractPipeline(const AbstractPipeline & rhs) = delete;
    AbstractPipeline(AbstractPipeline && rhs) = delete;
    AbstractPipeline & operator=(const AbstractPipeline & rhs) = delete;
    AbstractPipeline & operator=(AbstractPipeline && rhs) = delete;

    const std::string & name() const;
    void setName(const std::string & name);

    bool hasName() const;

    std::string asPrintable() const;

    bool isInitialized() const;
    void initialize();

    virtual void execute();

    virtual void addStage(AbstractStage * stage);

    void addParameter(AbstractData * parameter);
    void addParameter(const std::string & name, AbstractData * parameter);

    template <typename T>
    Data<T> * addConstantParameter(const T & value);

    void shareData(const AbstractData* data);
    void shareDataFrom(const AbstractInputSlot& slot);

    std::vector<AbstractInputSlot *> allInputs() const;
    std::vector<AbstractData *> allOutputs() const;

    AbstractData * findParameter(const std::string & name) const;
    std::vector<AbstractData *> findOutputs(const std::string & name) const;

    template <typename T>
    Data<T> * getParameter(const std::string & name) const;
    template <typename T>
    Data<T> * getParameter() const;

    template <typename T>
    Data<T> * getOutput(const std::string & name) const;
    template <typename T>
    Data<T> * getOutput() const;

    template<typename... Args>
    void addStages(AbstractStage * stage, Args... pipeline);

    const std::vector<AbstractStage *> & stages() const;
    const std::vector<AbstractData *> & parameters() const;

    std::set<AbstractData *> unusedParameters();


protected: 
    bool sortDependencies();
    void addStages();
    bool initializeStages();

    static bool tsort(std::vector<AbstractStage *> &stages);


protected:
    bool m_initialized;
    std::string m_name;
    std::vector<AbstractStage *> m_stages;
    std::vector<AbstractData *> m_constantParameters;
    std::vector<AbstractData *> m_parameters;
    std::vector<const AbstractData *> m_sharedData;
    bool m_dependenciesSorted;
};


} // namespace gloperate


#include <gloperate/pipeline/AbstractPipeline.hpp>
