#ifndef SAVEFILEDIALOG_HPP
#define SAVEFILEDIALOG_HPP

#include <gtkmm/filechooserdialog.h>

namespace Slicer {

class SaveFileDialog : public Gtk::FileChooserDialog
{
public:
	SaveFileDialog(Gtk::Window& parent);
	std::string getSavePath() const;
};

} // namespace Slicer

#endif // SAVEFILEDIALOG_HPP
