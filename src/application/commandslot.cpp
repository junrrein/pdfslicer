#include "commandslot.hpp"

namespace Slicer {

CommandSlot::CommandSlot(BackgroundThread& backgroundThread)
    : m_backgroundThread{backgroundThread}
{
    m_executeCommandDispatcher.connect(sigc::mem_fun(*this, &CommandSlot::executeCommand));
}

void CommandSlot::queueCommand(const std::function<void()>& command)
{
    if (m_commandToExecute != nullptr)
        throw std::runtime_error("There is already a queued command");

    m_commandToExecute = command;

    m_backgroundThread.push([this]() {
        m_executeCommandDispatcher.emit();
    });

    commandQueuedSignal.emit();
}

void CommandSlot::executeCommand()
{
    if (m_commandToExecute == nullptr)
        throw std::runtime_error("There is no queued command");

    m_commandToExecute();
    m_commandToExecute = nullptr;
}

} // namespace Slicer
