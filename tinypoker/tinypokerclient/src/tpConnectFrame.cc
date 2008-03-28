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

#include <wx/wx.h>
#include <wx/intl.h>

#include "tpConnectFrame.h"
#include "tpFrame.h"

tpConnectFrame::tpConnectFrame(tpFrame *parent, wxMenu *menuFile, const wxString& title, const wxPoint& pos, const wxSize& size, wxLocale& locale): wxFrame(parent, -1, title, pos, size), m_locale(locale) {
	m_parent = parent;
	m_menuFile = menuFile;

	m_menuFile->Enable(ID_Connect, false);
	m_menuFile->Enable(ID_Disconnect, true);
	m_parent->SetStatusText(_("Connected"));

	Centre();
}

void tpConnectFrame::OnConnect(wxCommandEvent& event) {
	Close(true);
}

BEGIN_EVENT_TABLE(tpConnectFrame, wxFrame)
	EVT_MENU(ID_ConnectClick, tpConnectFrame::OnConnect)
END_EVENT_TABLE()
