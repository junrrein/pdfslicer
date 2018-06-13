#ifndef COMMANDSLOT_HPP
#define COMMANDSLOT_HPP

#include "backgroundthread.hpp"
#include <glibmm/dispatcher.h>

namespace Slicer {

class CommandSlot {
public:
    CommandSlot(BackgroundThread& backgroundThread);

    void queueCommand(const std::function<void()>& command);
    sigc::signal<void> commandQueuedSignal;

private:
    BackgroundThread& m_backgroundThread;
    std::function<void()> m_commandToExecute = nullptr;
    Glib::Dispatcher m_executeCommandDispatcher;

    void executeCommand();
};

} // namespace Slicer

#endif // COMMANDSLOT_HPP
