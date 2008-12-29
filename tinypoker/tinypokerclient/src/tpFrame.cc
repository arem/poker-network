// Copyright (C) 2008 Thomas Cort <linuxgeek@gmail.com>
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
#include <wx/aboutdlg.h>
#include <wx/wizard.h>

#include "tpConnectionWizard.h"
#include "tpFrame.h"
#include "tpLog.h"
#include "tpApp.h"

tpFrame::tpFrame(const wxString& title, const wxPoint& pos, const wxSize& size, wxLocale& locale): wxFrame((wxFrame *)NULL, -1, title, pos, size), m_locale(locale) {
	ipp_init();
	m_sock = NULL;

	m_menuFile = new wxMenu;
	m_menuFile->Append(ID_Connect, _("&New Connection...\tCtrl+N"));
	m_menuFile->Append(ID_Disconnect, _("&Disconnect\tCtrl+D"));
	m_menuFile->Enable(ID_Disconnect, false);
	m_menuFile->AppendSeparator();
	m_menuFile->Append(ID_Exit, _("&Quit\tCtrl+Q"));

	wxMenu *menuHelp = new wxMenu;
	menuHelp->Append(ID_About, _("&About"));

	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(m_menuFile, _("&File"));
	menuBar->Append(menuHelp, _("&Help"));
	SetMenuBar(menuBar);

	CreateStatusBar();
	SetStatusText(_("Not Connected"));

	m_parent = new wxPanel(this, wxID_ANY);
	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
	m_log = new tpLog(m_parent);
	vbox->Add(m_log, 1, wxEXPAND | wxALL);
	m_parent->SetSizer(vbox);

	m_wiz = new tpConnectionWizard(this);

	Centre();
}

void tpFrame::OnAbout(wxCommandEvent& WXUNUSED(event)) {
	wxAboutDialogInfo info;

	info.SetName(_("TinyPoker Client"));
	info.SetVersion(_(VERSION));
	info.SetDescription(_("basic GUI designed for use with tinypokerd"));
	info.SetCopyright(_("(C) 2008 Thomas Cort <linuxgeek@gmail.com>"));
	info.SetWebSite(_("http://tinypoker.org/"));

	info.AddDeveloper(_("Thomas Cort <linuxgeek@gmail.com>"));

	info.SetLicence(
		_(
			"tinypokerclient is free software: you can redistribute it and/or modify\n"
			"it under the terms of the GNU General Public License as published by\n"
			"the Free Software Foundation, either version 3 of the License, or\n"
			"(at your option) any later version.\n"
			"\n"
			"tinypokerclient is distributed in the hope that it will be useful,\n"
			"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
			"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
			"GNU General Public License for more details.\n"
			"\n"
			"You should have received a copy of the GNU General Public License\n"
			"along with tinypokerclient.  If not, see <http://www.gnu.org/licenses/>.\n"
		)
	);

	wxAboutBox(info);
}

void tpFrame::OnConnect(wxCommandEvent& WXUNUSED(event)) {
	m_wiz->RunWizard(m_wiz->GetFirstPage());
}

void tpFrame::OnConnectionWizardFinished(wxWizardEvent& WXUNUSED(event)) {
	wxMessageBox(m_wiz->getHostname(), _("Hostname"));
	wxMessageBox(m_wiz->getUsername(), _("Username"));
	wxMessageBox(m_wiz->getPassword(), _("Password"));

	/* TODO validate input and do handshake */
}

void tpFrame::OnDisconnect(wxCommandEvent& WXUNUSED(event)) {
	if (m_sock != NULL) {
		ipp_disconnect(m_sock);
		ipp_free_socket(m_sock);
		m_sock = NULL;
	}

	m_menuFile->Enable(ID_Connect, true);
	m_menuFile->Enable(ID_Disconnect, false);

	SetStatusText(_("Not Connected"));
}

void tpFrame::log(const wxString& text) {
	m_log->log(text);
}

void tpFrame::OnExit(wxCommandEvent& event) {
	Close(true);
	OnDisconnect(event);
	ipp_exit();
}

BEGIN_EVENT_TABLE(tpFrame, wxFrame)
	EVT_MENU(ID_About, tpFrame::OnAbout)
	EVT_MENU(ID_Connect, tpFrame::OnConnect)
	EVT_MENU(ID_Disconnect, tpFrame::OnDisconnect)
	EVT_MENU(ID_Exit,  tpFrame::OnExit)

	EVT_WIZARD_FINISHED(wxID_ANY, tpFrame::OnConnectionWizardFinished)
END_EVENT_TABLE()

