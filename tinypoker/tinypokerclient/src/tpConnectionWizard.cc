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
#include <wx/wizard.h>
#include <wx/intl.h>

#include "tpConnectionWizard.h"
#include "tpConnectionWizardPage.h"

tpConnectionWizard::tpConnectionWizard(wxFrame *frame, bool useSizer) : wxWizard(frame, wxID_ANY, _("TinyPoker Connection Wizard"), NULL,wxDefaultPosition, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER) {
	m_page = m_tppage = new tpConnectionWizardPage(this);

	if (useSizer) {
		GetPageAreaSizer()->Add(m_page);
	}
}


wxWizardPage *tpConnectionWizard::GetFirstPage() {
	return m_page;
}

wxString tpConnectionWizard::getHostname() {
	return m_tppage->getHostname();
}

wxString tpConnectionWizard::getUsername() {
	return m_tppage->getUsername();
}

wxString tpConnectionWizard::getPassword() {
	return m_tppage->getPassword();
}
