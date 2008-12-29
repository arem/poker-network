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

#include <wx/wx.h>
#include <wx/intl.h>

#include "tpApp.h"
#include "tpFrame.h"

bool tpApp::OnInit() {
	m_locale.AddCatalog(wxT("tinypokerclient"));
	m_frame = new tpFrame(_("TinyPoker Client"), wxPoint(50,50), wxSize(450,340), m_locale);
	SetTopWindow(m_frame);

	m_frame->Show(true);
	return true;
}

void tpApp::log(const wxString& text) {
	m_frame->log(text);
}
