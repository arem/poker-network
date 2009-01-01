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

#include <wx/wx.h>
#include <wx/intl.h>

#include "tpLog.h"

tpLog::tpLog(wxPanel * parent) : wxPanel(parent, -1, wxPoint(-1, -1), wxSize(-1, -1), wxBORDER_SUNKEN) {
	wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
	m_lbox = new wxTextCtrl(this, -1, _(""), wxPoint(-1, -1), wxSize(-1, -1), wxTE_MULTILINE);
	hbox->Add(m_lbox, 1, wxEXPAND);
	this->SetSizer(hbox);
}

void tpLog::log(const wxString&  text) {
	m_lbox->AppendText(text);
}
