#ifndef OPENFILEDIALOG_H
#define OPENFILEDIALOG_H

#include <gtkmm/filechooserdialog.h>

namespace Slicer {

class OpenFileDialog : public Gtk::FileChooserDialog
{
public:
	OpenFileDialog(Gtk::Window& parent);
};

} // namespace Slicer

#endif // OPENFILEDIALOG_H
