// Copyright (C) 2008 Thomas Cort <tom@tomcort.com>
//
// This file is part of tinypokerclient.
//
// tinypokerclient is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// tinypokerclient is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with tinypokerclient.  If not, see <http://www.gnu.org/licenses/>.

#include <tinypoker.h>
#include <wx/wx.h>
#include <wx/intl.h>

#include "tpFrame.h"

tpFrame::tpFrame(const wxString& title, const wxPoint& pos, const wxSize& size, wxLocale& locale): wxFrame((wxFrame *)NULL, -1, title, pos, size), m_locale(locale) {
	ipp_init();

	wxMenu *menuFile = new wxMenu;
	menuFile->Append(ID_Exit, _("&Quit"));

	wxMenu *menuHelp = new wxMenu;
	menuHelp->Append(ID_About, _("&About"));

	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(menuFile, _("&File"));
	menuBar->Append(menuHelp, _("&Help"));
	SetMenuBar(menuBar);

	CreateStatusBar();
	SetStatusText(_("Not Connected"));
}

void tpFrame::OnAbout(wxCommandEvent& WXUNUSED(event)) {
	wxMessageBox(_("TinyPoker Client 0.0.0\nCopyright © 2008 Thomas Cort <tom@tomcort.com>"), _("About TinyPoker Client"), wxOK | wxICON_INFORMATION);
}

void tpFrame::OnExit(wxCommandEvent& WXUNUSED(event)) {
	ipp_exit();

	Close(true);
}

BEGIN_EVENT_TABLE(tpFrame, wxFrame)
	EVT_MENU(ID_About, tpFrame::OnAbout)
	EVT_MENU(ID_Exit,  tpFrame::OnExit)
END_EVENT_TABLE()

