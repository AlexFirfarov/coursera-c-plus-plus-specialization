#include <iostream>
#include <string>

void SendSms(const std::string& number, const std::string& message) {
    std::cout << "Send '" << message << "' to number " << number << std::endl;
}

void SendEmail(const std::string& email, const std::string& message) {
    std::cout << "Send '" << message << "' to e-mail "  << email << std::endl;
}

class INotifier {
public:
    virtual void Notify(const std::string& message) = 0;
};

class SmsNotifier : public INotifier {
public:
    SmsNotifier(const std::string& phone_number) : phone_number_(phone_number) {}
    void Notify(const std::string& message) override {
        SendSms(phone_number_, message);
    }
private:
    std::string phone_number_;
};

class EmailNotifier : public INotifier {
public:
    EmailNotifier(const std::string& email_address) : email_address_(email_address) {}
    void Notify(const std::string& message) override {
        SendEmail(email_address_, message);
    }
private:
    std::string email_address_;
};

void Notify(INotifier& notifier, const std::string& message) {
    notifier.Notify(message);
}


int main() {
    SmsNotifier sms{"+7-495-777-77-77"};
    EmailNotifier email{"na-derevnyu@dedushke.ru"};

    Notify(sms, "I have White belt in C++");
    Notify(email, "And want a Yellow one");
    return 0;
}
