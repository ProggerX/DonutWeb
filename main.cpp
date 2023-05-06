#include<Wt/WText.h>
#include<vector>
#include<fstream>
#include<Wt/WTemplate.h>
#include<Wt/WApplication.h>
#include<Wt/WLineEdit.h>
#include<Wt/WPushButton.h>
#include<Wt/WContainerWidget.h>
using namespace std;
struct message 
{
	string author, text;
};
vector<message> recive_message(string user)
{
	vector<message> vec;
	ofstream ofile(user, ios::app);
	ofile.close();
	fstream file(user);
	string a, t;
	while(getline(file, a))
	{
		getline(file, t);
		message msg;
		msg.author = a;
		msg.text = t;
		vec.push_back(msg);
	}
	file.close();
	return vec;
}
void send_message(string author, string reciver, string text)
{
	ofstream ofile(reciver, ios::app);
	ofile << author << '\n' << text << '\n';
	ofile.close();	
}
class LoginWidget : public Wt::WContainerWidget
{
		string username;
		Wt::WLineEdit *enterer;
		Wt::WPushButton *button;
	public:
		LoginWidget()
		{
			enterer = addWidget(make_unique<Wt::WLineEdit>("username"));
			button = addWidget(make_unique<Wt::WPushButton>("Login"));	
		}
		friend class MyMessenger;
};
class MessageSender : public Wt::WContainerWidget
{
		string curruser;
		Wt::WLineEdit *user, *mstext;
		Wt::WPushButton *send_button;
	public:
		void send() {
			send_message(curruser, user->text().toUTF8(), mstext->text().toUTF8());
			user->setText("whom");
			mstext->setText("message text");
		};
		MessageSender(string curruser) : curruser(curruser)
		{
			user = addWidget(make_unique<Wt::WLineEdit>("whom"));
			mstext = addWidget(make_unique<Wt::WLineEdit>("message text"));
			send_button = addWidget(make_unique<Wt::WPushButton>("Send"));
			send_button->clicked().connect(this, &MessageSender::send);
		}
};
class MessagesList : public Wt::WContainerWidget
{
		string user;
		vector<message> vec;
		Wt::WTemplate *list;
		Wt::WPushButton *refresh;
	public:
		void refresh_action()
		{
			//cout << "user: " << str << '\n';
			list->setTemplateText("");
			string str = "";
			for(int i = recive_message(user).size() - 1; i >= 0; i--)
			{
				vector<message> a = recive_message(user);
				vec = a;
				str += a[i].author + ": ";
				str += a[i].text;
				str += '\n';
				cout << "cycle\n";
				list->setTemplateText(list->templateText() + "<p>" + a[i].author + ": " + a[i].text + "</p>");
			}
			
			//cout << str;
		}
		MessagesList(string userr) : user(userr)
		{
			string str;
			//cout << "user: " << user << '\n';
			for(int i = recive_message(user).size() - 1; i >= 0; i--)
			{
				vector<message> a = recive_message(user);
				vec = a;
				str += a[i].author + ": ";
				str += a[i].text;
				str += '\n';
			}
			refresh = addWidget(make_unique<Wt::WPushButton>("Refresh"));
			refresh->clicked().connect(this, &MessagesList::refresh_action);
			list = addWidget(make_unique<Wt::WTemplate>());
			refresh_action();
		}
};
class MyMessenger : public Wt::WApplication
{
		LoginWidget *login_widget;
		MessageSender *sender_widget;
		MessagesList *list_widget;
		Wt::WString current_user;
		vector<message> vec;
	public:
		MyMessenger(const Wt::WEnvironment& env);
};
MyMessenger::MyMessenger(const Wt::WEnvironment& env) : Wt::WApplication(env)
{
	login_widget = root()->addWidget(make_unique<LoginWidget>());
	Wt::WText *hello = root()->addWidget(make_unique<Wt::WText>());
	auto login = [this]{
		current_user = login_widget->enterer->text();
		login_widget->hide();
		Wt::WText *logined_text = root()->addWidget(make_unique<Wt::WText>("logined as: " + current_user));
		root()->addWidget(make_unique<Wt::WBreak>());
		root()->addWidget(make_unique<Wt::WBreak>());
		recive_message(current_user.toUTF8());
		sender_widget =	root()->addWidget(make_unique<MessageSender>(current_user.toUTF8()));
		root()->addWidget(make_unique<Wt::WBreak>());
		root()->addWidget(make_unique<Wt::WBreak>());
		list_widget =	root()->addWidget(make_unique<MessagesList>(current_user.toUTF8()));
    };
	login_widget->button->clicked().connect(login);
	hello->setText("DonutWeb v1.0");
	root()->addWidget(make_unique<Wt::WBreak>());
}
int main(int argc, char **argv)
{
	//send_message("proggerx", "usr", "how are u");
	for(auto i : recive_message("usr"))
	{
		cout << i.author << ": " << i.text << '\n';
	}
	return Wt::WRun(argc, argv, [](const Wt::WEnvironment& env) {
      return std::make_unique<MyMessenger>(env);
    });
}
