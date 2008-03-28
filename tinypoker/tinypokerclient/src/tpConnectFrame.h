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

#ifndef __TPCONNECTFRAME_H
#define __TPCONNECTFRAME_H

#include <wx/wx.h>
#include <wx/intl.h>

#include "tpFrame.h"

class tpConnectFrame: public wxFrame {
	public:
		tpConnectFrame(tpFrame *parent, wxMenu *menuFile, const wxString& title, const wxPoint& pos, const wxSize& size, wxLocale& locale);

		void OnConnect(wxCommandEvent& event);

	private:
		DECLARE_EVENT_TABLE()
		tpFrame *m_parent;
		wxLocale& m_locale;
		wxMenu *m_menuFile;
};

enum {
	ID_ConnectClick = 1
};

#endif
