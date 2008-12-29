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

#include "tpConnectionWizardPage.h"

tpConnectionWizardPage::tpConnectionWizardPage(wxWizard *parent) : wxWizardPageSimple(parent) {

	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *hostname_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *username_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *password_sizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *hostname_label = new wxStaticText(this, -1, _("Hostname"));
	hostname_sizer->Add(hostname_label, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT | wxALL, 10);
	m_hostname = new wxTextCtrl(this, -1);
	hostname_sizer->Add(m_hostname, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 10);
	sizer->Add(hostname_sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxEXPAND | wxALL, 25);

	wxStaticText *username_label = new wxStaticText(this, -1, _("Username"));
	username_sizer->Add(username_label, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT | wxALL, 10);
	m_username = new wxTextCtrl(this, -1);
	username_sizer->Add(m_username, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 10);
	sizer->Add(username_sizer, 0, wxALIGN_CENTER_HORIZONTAL |wxEXPAND | wxALL, 25);

	wxStaticText *password_label = new wxStaticText(this, -1, _("Password"));
	password_sizer->Add(password_label, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT | wxALL, 10);
	m_password = new wxTextCtrl(this, -1, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
	password_sizer->Add(m_password, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 10);
	sizer->Add(password_sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxEXPAND | wxALL, 25);

	sizer->SetSizeHints(this);
	SetSizer(sizer);
}

wxString tpConnectionWizardPage::getUsername() {
	return m_username->GetValue();
}

wxString tpConnectionWizardPage::getHostname() {
	return m_hostname->GetValue();
}

wxString tpConnectionWizardPage::getPassword() {
	return m_password->GetValue();
}

