#pragma once
#include <wx/wx.h>
#include <wx/socket.h>

#include <XFaceApp/Task.h>
#include <XFaceApp/Notification.h>

// Define a new Dialog type: this is going to be our main Dialog
class FaceClientSimpleDlg :
	public wxDialog
{
	enum WIDGET_IDS
	{
		ID_SEND_SCRIPT,
		ID_PLAY,
		ID_CONNECT,
		ID_DISCONNECT,
		ID_SOCKET,
		ID_KILL
	};

	wxSocketClient* m_pSocket;
	bool m_bConnected;
	unsigned short m_clientId;
	unsigned int m_taskCount;
	wxString m_IPAddress;
	unsigned int m_port;
	wxString ReadFromSocket(wxSocketBase *sock);
	void Connect(bool bConnect);
	void OnNotification(const XFaceApp::Notification& note);
	bool SendTask(const XFaceApp::Task& task);
protected:
	void OnSocketEvent(wxSocketEvent& event);
	void OnConnect( wxCommandEvent &event);
	void OnDisconnect( wxCommandEvent &event);
	void OnKill( wxCommandEvent &event);
	void OnPlay( wxCommandEvent &event);
	void OnSendScript( wxCommandEvent &event);
	void OnClose(wxCloseEvent& WXUNUSED(event));
public:
	FaceClientSimpleDlg(wxWindow* parent, wxWindowID id, const wxString& title, 
		const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, 
		long style = wxDEFAULT_DIALOG_STYLE, const wxString& name = _("dialogBox"));
	virtual ~FaceClientSimpleDlg(void);

private:
	DECLARE_CLASS(FaceClientSimpleDlg)
	// any class wishing to process wxWindows events must use this macro
	DECLARE_EVENT_TABLE()
};
