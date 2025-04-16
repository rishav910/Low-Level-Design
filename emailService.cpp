#include <bits/stdc++.h>
using namespace std;
class Email;
/*
    Inbox
    Properties:
    - vector<Email*>emails
    Methods:
    + searchEmail()
    + receiveEmail()
    + showEmails()
*/
class Inbox {
    vector<Email*>emails;
    // Should store drafts somewhere
    
    public:
    Email* searchEmail(string em);
    void receiveEmail(Email *email);
    void showEmails();
};
/*
    User
    Properties:
    - emailID
    - password
    - Inbox
    Methods:
    + getInbox()
*/
class User {
    string emailID;
    string password;
    Inbox *inbox;
    public:
    User(string email, string pass):emailID(email), password(pass), inbox(new Inbox()){}
    
    Inbox* getInbox() {
        return inbox;
    }
};
/*
    Email
    Properties:
    - read
    - draft
    + body
    - User *sender, *receiver
    - sentTime
    - attachment
    Methods:
    + makeDraft()
    + markSent()
    + getReceiver()
    + showEmail()
    + getType()
*/
class Email {
    bool read;
    bool draft;
    bool sent;
    User *sender, *receiver;
    time_t sentTime;
    string attachment;
    public:
    string body;
    Email(string body, User *a, User *b, time_t sentT, string attachment="") {
        read = false;
        draft = false;
        sent = false;
        this->body = body;
        this->sentTime = sentT;
        sender = a;
        receiver = b;
        this->attachment = attachment;
    }
    void makeDraft() {
        if(sent == true) {
            cout<<"Email already sent"<<endl;
            return;
        }
        draft = true;
    }
    void markSent() {
        cout<<"Email sent"<<endl;
        sent = true;
    }
    User* getReceiver() {
        return receiver;
    }
    void showEmail() {
        // Can add more details here
        cout<<"Email time: "<<ctime(&sentTime)<<endl;
        cout<<"Email body: \n"<<body<<endl;
    }
    virtual string getType() {
        return draft ? "draft":"email";
    }
};
/*
    Inbox
    Methods definition:
    + searchEmail()
    + receiveEmail()
    + showEmails()
*/
Email* Inbox::searchEmail(string em) {
    for(auto x:emails) {
        string emailBody = x->body;
        if(emailBody.find(em) != string::npos) {
            return x;
        }
    }
    return NULL;
}
void Inbox::receiveEmail(Email *email) {
    emails.push_back(email);
}
void Inbox::showEmails() {
    if(emails.size() == 0) {
        cout<<"Empty inbox"<<endl;
        return;
    }
    for(auto x:emails) {
        x->showEmail();
    }
}


/*
    EmailManager
    Properties:
    Methods:
    + composeEmail()
    + sendEmail()
*/
class EmailManager {
    public:
    Email* composeEmail(User *sender, User *receiver, string body, string attachment) {
        // Opens UI
        Email *email = new Email(body, sender, receiver, time(NULL), attachment);
        return email;
    }
    void sendEmail(Email *email) {
        User *receiver = email->getReceiver();
        receiver->getInbox()->receiveEmail(email);
        email->markSent();
    }
    void makeEmailDraft(Email *email) {
        email->makeDraft();
    }
};
int main() {
    User *user1 = new User("rishavsaha@gmail.com","hello");
    User *user2 = new User("barunsaha37@gmail.com","hello2");
    
    EmailManager *m = new EmailManager;
    Email *e1 = m->composeEmail(user1, user2, "hey bro come", "no attachment");
    m->sendEmail(e1);
    Email *e2 = m->composeEmail(user1, user2, "hey bro plan cancel", "reason.txt");
    m->sendEmail(e2);
    
    // Get user2 inbox
    user2->getInbox()->showEmails();
    
    // Get search results
    Email *email = user2->getInbox()->searchEmail("bro");
    if(email) {
        cout<<"Email found"<<endl;
    }
    
    delete user1; delete user2;
    delete m; delete e1; delete e2;
    
    // time(NULL) -> Number of second elapsed since 00:00:00 UTC on January 1, 1970
    time_t x = time(NULL);
    // Readable date and time -> ctime(&var)
    cout<<"TIME: "<<x<<"  &   "<<ctime(&x)<<endl;
}