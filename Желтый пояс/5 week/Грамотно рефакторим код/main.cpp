#include <iostream>
#include <string>
#include <vector>

class Human {
public:
    Human(const std::string& name, const std::string& type) : Name(name), Type(type) {}
    virtual void Walk(const std::string& destination) const {
        PrintTypeAndName();
        std::cout << " walks to: " << destination << std::endl;
    }
    std::string GetName() const {
        return Name;
    }
    std::string GetType() const {
        return Type;
    }
    virtual ~Human() {}

protected:
    void PrintTypeAndName() const {
        std::cout << Type << ": " << Name;
    }

private:
    const std::string Name;
    const std::string Type;
};

class Student : public Human {
public:
    Student(const std::string& name, const std::string& favouriteSong) : Human(name, "Student"), FavouriteSong(favouriteSong) {}
    void Walk(const std::string& destination) const override {
        ::Human::Walk(destination);
        SingSong();
    }
    void SingSong() const {
        PrintTypeAndName();
        std::cout << " sings a song: " << FavouriteSong << std::endl;
    }
    void Learn() const {
        PrintTypeAndName();
        std::cout << " learns" << std::endl;
    }
    std::string GetFavouriteSong() const {
        return FavouriteSong;
    }

private:
    const std::string FavouriteSong;
};

class Teacher : public Human {
public:
    Teacher(const std::string& name, const std::string& subject) : Human(name, "Teacher"), Subject(subject) {}
    void Teach() const {
        PrintTypeAndName();
        std::cout << " teaches: " << Subject << std::endl;
    }
    std::string GetSubject() const {
        return Subject;
    }

private:
    const std::string Subject;
};

class Policeman : public Human {
public:
    Policeman(const std::string& name) : Human(name, "Policeman") {}
    void Check(const Human& human) const {
        const std::string human_type = human.GetType();

        PrintTypeAndName();
        std::cout << " checks " << human_type << ". " << human_type << "'s name is: " << human.GetName() << std::endl;
    }
};

void VisitPlaces(const Human& human, const std::vector<std::string>& places) {
    for (const std::string& p : places) {
        human.Walk(p);
    }
}

int main() {
    Teacher t("Jim", "Math");
    Student s("Ann", "We will rock you");
    Policeman p("Bob");

    VisitPlaces(t, {"Moscow", "London"});
    p.Check(s);
    VisitPlaces(s, {"Moscow", "London"});
    return 0;
}