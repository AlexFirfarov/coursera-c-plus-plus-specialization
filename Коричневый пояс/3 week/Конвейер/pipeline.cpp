#include "test_runner.h"
#include <functional>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

struct Email {
  string from;
  string to;
  string body;
};

class Worker {
public:
  virtual ~Worker() = default;
  virtual void Process(unique_ptr<Email> email) = 0;
  virtual void Run() {
    // только первому worker-у в пайплайне нужно это имплементировать
    throw logic_error("Unimplemented");
  }

protected:
  // реализации должны вызывать PassOn, чтобы передать объект дальше
  // по цепочке обработчиков
  void PassOn(unique_ptr<Email> email) const {
    if (next_worker) {
      next_worker->Process(move(email));
    }
  }

  unique_ptr<Worker> next_worker = nullptr;

public:
  void SetNext(unique_ptr<Worker> next) {
    next_worker = move(next);
  }
};


class Reader : public Worker {
public:
  Reader(istream& stream) : stream_(stream) {}

  void Run() override {
    string from, to, body;
    while (getline(stream_, from, '\n'), 
           getline(stream_, to, '\n'),
           getline(stream_, body, '\n')) {
      unique_ptr<Email> email(new Email);
      email->from = move(from);
      email->to = move(to);
      email->body = move(body);
      PassOn(move(email));
    }
  }

  void Process(unique_ptr<Email> email) override {
    PassOn(move(email));
  }

private:
  istream& stream_;
};


class Filter : public Worker {
public:
  using Function = function<bool(const Email&)>;

public:
  Filter(Function predicate) : predicate_(move(predicate)) {}

  void Process(unique_ptr<Email> email) override {
    if (predicate_(*email)) {
      PassOn(move(email));
    }
  }

private:
  Function predicate_;
};


class Copier : public Worker {
public:
  Copier(string recipient) : recipient_(move(recipient)) {}

  void Process(unique_ptr<Email> email) override {
    unique_ptr<Email> copy_email;
    if (email->to != recipient_) {
      copy_email = make_unique<Email>(*email);
      copy_email->to = recipient_;
    }
    PassOn(move(email));
    if (copy_email) {
      PassOn(move(copy_email));
    }
  }

private:
  string recipient_;
};


class Sender : public Worker {
public:
  Sender(ostream& stream) : stream_(stream) {}

  void Process(unique_ptr<Email> email) override {
    stream_ << email->from << '\n'
            << email->to << '\n'
            << email->body << '\n';
    PassOn(move(email));
  }

private:
  ostream& stream_;
};


// реализуйте класс
class PipelineBuilder {
public:
  // добавляет в качестве первого обработчика Reader
  explicit PipelineBuilder(istream& in) : head(make_unique<Reader>(in)), last(head.get()) {}

  // добавляет новый обработчик Filter
  PipelineBuilder& FilterBy(Filter::Function filter) {
    SetNext(make_unique<Filter>(move(filter)));
    return *this;
  }

  // добавляет новый обработчик Copier
  PipelineBuilder& CopyTo(string recipient) {
    SetNext(make_unique<Copier>(move(recipient)));
    return *this;
  }

  // добавляет новый обработчик Sender
  PipelineBuilder& Send(ostream& out) {
    SetNext(make_unique<Sender>(out));
    return *this;
  }

  // возвращает готовую цепочку обработчиков
  unique_ptr<Worker> Build() {
    return move(head);
  }

private:
  void SetNext(unique_ptr<Worker> next) {
    Worker* prev_last = last;
    last = next.get();
    prev_last->SetNext(move(next));
  }

  unique_ptr<Worker> head;
  Worker* last;
};


void TestSanity() {
  string input = (
    "erich@example.com\n"
    "richard@example.com\n"
    "Hello there\n"

    "erich@example.com\n"
    "ralph@example.com\n"
    "Are you sure you pressed the right button?\n"

    "ralph@example.com\n"
    "erich@example.com\n"
    "I do not make mistakes of that kind\n"
  );
  istringstream inStream(input);
  ostringstream outStream;

  PipelineBuilder builder(inStream);
  builder.FilterBy([](const Email& email) {
    return email.from == "erich@example.com";
  });
  builder.CopyTo("richard@example.com");
  builder.Send(outStream);
  auto pipeline = builder.Build();

  pipeline->Run();

  string expectedOutput = (
    "erich@example.com\n"
    "richard@example.com\n"
    "Hello there\n"

    "erich@example.com\n"
    "ralph@example.com\n"
    "Are you sure you pressed the right button?\n"

    "erich@example.com\n"
    "richard@example.com\n"
    "Are you sure you pressed the right button?\n"
  );

  ASSERT_EQUAL(expectedOutput, outStream.str());
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSanity);
  return 0;
}
