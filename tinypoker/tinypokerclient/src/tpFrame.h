// Copyright (C) 2008, 2009 Thomas Cort <linuxgeek@gmail.com>
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

#ifndef __TPFRAME_H
#define __TPFRAME_H

#include <tinypoker.h>

#include <wx/wx.h>
#include <wx/wizard.h>
#include <wx/intl.h>

#include "tpConnectionWizard.h"
#include "tpLog.h"

class tpFrame: public wxFrame {
	public:
		tpFrame(const wxString& title, const wxPoint& pos, const wxSize& size, wxLocale& locale);

		void log(const wxString& text);

		void OnAbout(wxCommandEvent& event);
		void OnConnect(wxCommandEvent& event);
		void OnDisconnect(wxCommandEvent& event);
		void OnExit(wxCommandEvent& event);

		void OnConnectionWizardFinished(wxWizardEvent& event);
	private:
		DECLARE_EVENT_TABLE()
		wxLocale& m_locale;

		wxPanel *m_parent;
		wxMenu *m_menuFile;
		tpLog *m_log;
		ipp_socket *m_sock;
		tpConnectionWizard *m_wiz;
};

enum {
	ID_About = 1,
	ID_Connect,
	ID_Disconnect,
	ID_Exit
};

#endif
