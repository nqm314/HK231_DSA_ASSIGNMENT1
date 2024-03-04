#include "main.h"

class imp_res : public Restaurant {
private:
  class Queue {
  private:
    customer *Qhead, *Qtail;
    int tempsize;

  public:
    Queue(customer *head = nullptr, customer *tail = nullptr)
        : tempsize(0), Qhead(head), Qtail(tail) {}
    virtual ~Queue() { clear(); }
    void clear() {
      if (!Qhead)
        return;
      customer *tmp = Qhead;
      while (Qhead) {
        Qhead = Qhead->next;
        delete tmp;
        tmp = Qhead;
      }
      delete tmp;
    }
    int getSize() { return this->tempsize; }
    bool isEmpty() { return !this->tempsize; }
    bool isFull() { return this->tempsize == MAXSIZE; }
    customer *getQhead() { return this->Qhead; }
    customer *getQtail() { return this->Qtail; }
    bool checkExist(string name) {
      if (!Qhead && !Qtail)
        return false;
      customer *tmp = Qhead;
      while (tmp) {
        if (tmp->name == name)
          return true;
        tmp = tmp->next;
      }
      return false;
    }
    void enqueue(string name, int energy) {
      if (this->isFull())
        return;
      if (this->isEmpty()) {
        customer *newnode = new customer(name, energy, NULL, NULL);
        this->Qhead = this->Qtail = newnode;
        ++this->tempsize;
        return;
      }
      customer *newnode = new customer(name, energy, this->Qtail, NULL);
      this->Qtail->next = newnode;
      this->Qtail = newnode;
      ++this->tempsize;
    }
    void dequeue() {
      if (this->isEmpty())
        return;
      if (this->tempsize == 1) {
        this->Qhead = nullptr;
        this->Qtail = nullptr;
        --this->tempsize;
        return;
      }
      customer *tmp = this->Qhead;
      if (this->Qhead->next)
        this->Qhead->next->prev = nullptr;
      this->Qhead = this->Qhead->next;
      // delete tmp;
      --this->tempsize;
    }
    customer *pop() {
      if (this->isEmpty())
        return nullptr;
      return this->Qhead;
    }
    void remove(customer *c) {
      if (this->tempsize == 1 && Qhead == c) {
        this->Qhead = nullptr;
        this->Qtail = nullptr;
        --this->tempsize;
        return;
      }
      if (this->Qhead == c) {
        this->Qhead->next->prev = nullptr;
        this->Qhead = this->Qhead->next;
        --this->tempsize;
        return;
      } else if (this->Qtail == c) {
        this->Qtail->prev->next = nullptr;
        this->Qtail = this->Qtail->prev;
        --this->tempsize;
        return;
      }
      c->prev->next = c->next;
      c->next->prev = c->prev;
      c->prev = nullptr;
      c->next = nullptr;
      c = nullptr;
      --this->tempsize;
      return;
    }
    void swap(customer *a, customer *b) {
      customer *tmp = new customer(a->name, a->energy, nullptr, nullptr);
      a->name = b->name;
      a->energy = b->energy;
      b->name = tmp->name;
      b->energy = tmp->energy;
    }
    customer *findPos() {
      customer *tmp = this->Qhead, *result;
      int max = abs(this->Qhead->energy);
      while (tmp) {
        if (abs(tmp->energy) >= max) {
          max = abs(tmp->energy);
          result = tmp;
        }
        tmp = tmp->next;
      }
      return result;
    }
    int sumOfSorcerers() {
      int res = 0;
      customer *tmp = this->Qhead;
      while (tmp) {
        if (tmp->energy > 0)
          res += tmp->energy;
        tmp = tmp->next;
      }
      return res;
    }
    int sumOfCurses() {
      int res = 0;
      customer *tmp = this->Qhead;
      while (tmp) {
        if (tmp->energy < 0)
          res += tmp->energy;
        tmp = tmp->next;
      }
      return res;
    }
    void removeAllSorcerers() {
      customer *tmp = this->Qhead;
      while (tmp) {
        customer *ne = tmp->next;
        if (tmp->energy > 0)
          this->remove(tmp);
        tmp = ne;
      }
    }
    void removeAllCurses() {
      customer *tmp = this->Qhead;
      while (tmp) {
        customer *ne = tmp->next;
        if (tmp->energy < 0)
          this->remove(tmp);
        tmp = ne;
      }
    }
    void printAllSorcerers() { // by order from tail of queue to head of queue
      customer *tmp = this->Qtail;
      while (tmp) {
        if (tmp->energy > 0)
          tmp->print();
        tmp = tmp->prev;
      }
    }
    void printAllCurses() {
      customer *tmp = this->Qtail;
      while (tmp) {
        if (tmp->energy < 0)
          tmp->print();
        tmp = tmp->prev;
      }
    }

    // cần chỉnh sửa thêm
    void Qprint() {
      customer *tmp = this->Qhead;
      while (tmp) {
        tmp->print();
        tmp = tmp->next;
      }
    }
  };

private:
  customer *Rhead, *Rcurrent;
  Queue *queue, *orderQ, *oR;
  int tempsize;

public:
  imp_res(customer *Rhead = nullptr, customer *Rcurrent = nullptr)
      : tempsize(0), Rhead(Rhead), Rcurrent(Rcurrent) {
    // Rtail->next = Rhead;
    // Rhead->prev = Rtail;
    queue = new Queue();
    orderQ = new Queue();
    oR = new Queue();
  }
  ~imp_res() {
    while (tempsize--) {
      customer *tmp = this->Rhead;
      this->Rhead = this->Rhead->next;
      delete tmp;
    }
    delete queue;
    delete orderQ;
    delete oR;
  }

  // helper function for RED method
  void insert(customer *newcus) {
    if (this->tempsize == MAXSIZE) {
      queue->enqueue(newcus->name, newcus->energy);
      oR->enqueue(newcus->name, newcus->energy);
      return;
    }
    orderQ->enqueue(newcus->name, newcus->energy);
    if (this->tempsize == 0) {
      customer *newnode =
          new customer(newcus->name, newcus->energy, nullptr, nullptr);
      this->Rhead = this->Rcurrent = newnode;
      ++this->tempsize;
      return;
    }
    if (this->tempsize == 1) {
      customer *newnode =
          new customer(newcus->name, newcus->energy, nullptr, nullptr);
      this->Rcurrent->next = newnode;
      this->Rcurrent->prev = newnode;
      newnode->next = this->Rcurrent;
      newnode->prev = this->Rcurrent;
      this->Rcurrent = newnode;
      ++this->tempsize;
      return;
    }
    customer *newnode =
        new customer(newcus->name, newcus->energy, nullptr, nullptr);
    if (newnode->energy >= this->Rcurrent->energy) {
      newnode->next = this->Rcurrent->next;
      this->Rcurrent->next->prev = newnode;
      this->Rcurrent->next = newnode;
      newnode->prev = this->Rcurrent;
      this->Rcurrent = newnode;
    } else {
      newnode->prev = this->Rcurrent->prev;
      this->Rcurrent->prev->next = newnode;
      this->Rcurrent->prev = newnode;
      newnode->next = this->Rcurrent;
      this->Rcurrent = newnode;
    }
    ++this->tempsize;
  }
  bool checkExist(string name) {
    if (!Rhead)
      return false;
    if (Rhead->name == name)
      return true;
    customer *tmp = this->Rhead->next;
    while (tmp != this->Rhead && tmp) {
      if (tmp->name == name)
        return true;
      tmp = tmp->next;
    }
    return false;
  }
  // function for RED method
  void RED(string name, int energy) {
    // cout << "red\n";
    if (energy == 0)
      return;
    if (this->checkExist(name) || queue->checkExist(name))
      return;
    customer *cus = new customer(name, energy, nullptr, nullptr);
    if (this->tempsize < MAXSIZE / 2)
      this->insert(cus);
    else if (this->tempsize >= MAXSIZE)
      this->insert(cus);
    else {
      int maxAbsDiff = abs(cus->energy - this->Rcurrent->energy);
      customer *res = this->Rcurrent;
      customer *tmp = this->Rcurrent->next;
      while (tmp != this->Rcurrent) {
        if (abs(cus->energy - tmp->energy) > maxAbsDiff) {
          maxAbsDiff = abs(cus->energy - tmp->energy);
          res = tmp;
        }
        tmp = tmp->next;
      }
      this->Rcurrent = res;
      this->insert(cus);
    }
  }

  // helper function for BLUE method
  void remove(string name) {
    // only one element
    if (this->tempsize == 1) {
      if (this->Rcurrent->name == name) {
        this->Rhead = nullptr;
        this->Rcurrent = nullptr;
        this->tempsize--;
      }
      return;
    }
    if (this->Rhead->name == name) {
      if (this->Rhead->energy > 0)
        this->Rcurrent = this->Rhead->next;
      else
        this->Rcurrent = this->Rhead->prev;
      this->Rhead->prev->next = this->Rhead->next;
      this->Rhead->next->prev = this->Rhead->prev;
      this->Rhead = this->Rcurrent;
      this->tempsize--;
      return;
    }
    customer *tmp = this->Rhead->next;
    while (tmp != this->Rhead) {
      if (tmp->name == name) {
        if (tmp->energy > 0)
          this->Rcurrent = tmp->next;
        else
          this->Rcurrent = tmp->prev;
        tmp->prev->next = tmp->next;
        tmp->next->prev = tmp->prev;
        this->tempsize--;
        return;
      }
      tmp = tmp->next;
    }
  }
  // function for BLUE method
  void BLUE(int num) {
    // cout << "blue" << num << endl;
    if (this->tempsize == 0)
      return;
    if (num >= tempsize || num >= MAXSIZE)
      num = tempsize;
    while (num--) {
      customer *cusToRem = orderQ->pop();
      orderQ->dequeue();
      if (cusToRem)
        this->remove(cusToRem->name);
    }
    if (!queue->isEmpty()) {
      while (!queue->isEmpty() && this->tempsize < MAXSIZE) {
        customer *newnode = queue->pop();
        queue->dequeue();
        this->RED(newnode->name, newnode->energy);
        customer *tmp = oR->getQhead();
        while (tmp) {
          if (tmp->name == newnode->name) {
            oR->remove(tmp);
            break;
          }
          tmp = tmp->next;
        }
      }
    }
  }


  customer *getCustomer(customer *s, int index) {
    if (!s)
      return s;
    while (index-- && s) {
      s = s->next;
    }
    return s;
  }
  bool comp(customer *s, int i, int j) {
    customer *x = getCustomer(s, i);
    customer *y = getCustomer(s, j);
    if (!x || !y)
      return false;
    if (abs(x->energy) > abs(y->energy))
      return true;
    else if (abs(x->energy) < abs(y->energy))
      return false;
    customer *tmp = oR->getQtail();
    while (tmp) {
      if (tmp == x)
        return false;
      else if (tmp == y)
        return true;
      tmp = tmp->prev;
    }
    return false;
  }
  void inssort2(customer *s, int n, int incr, int &step) {
    for (int i = incr; i < n; i += incr) {
      for (int j = i; j >= incr && comp(s, j, j - incr); j -= incr) {
        customer *x = getCustomer(s, j);
        customer *y = getCustomer(s, j - incr);
        queue->swap(x, y);
        step++;
      }
    }
  }
  // function for method PURPLE
  void PURPLE() {
    // cout << "purple\n";
    if (queue->isEmpty() || queue->getSize() == 1)
      return;
    customer *pos = queue->findPos();
    int step = 0, n = 1;
    customer *curr = queue->getQhead();
    while (curr->name != pos->name) {
      curr = curr->next;
      n++;
    }
    for (int i = n / 2; i > 2; i /= 2) {
      for (int j = 0; j < i; j++) {
        customer *s = getCustomer(queue->getQhead(), j);
        inssort2(s, n - j, i, step);
      }
    }
    inssort2(queue->getQhead(), n, 1, step);
    BLUE(step % MAXSIZE);
  }

  // helper function for REVERSAL method
  void swap(customer *a, customer *b) {
    customer *tmp = new customer(a->name, a->energy, nullptr, nullptr);
    a->name = b->name;
    a->energy = b->energy;
    b->name = tmp->name;
    b->energy = tmp->energy;
  }
  void reversal() {
    string nameOfCurr = this->Rcurrent->name;
    customer *left1 = this->Rcurrent, *right1 = this->Rcurrent->next;
    int step1 = 0;
    while (left1 != right1 && step1 < this->tempsize) {
      if (left1->energy < 0 && right1->energy < 0) {
        swap(left1, right1);
        left1 = left1->prev;
        right1 = right1->next;
        step1 += 2;
        continue;
      }
      if (left1->energy > 0)
        left1 = left1->prev, step1++;
      if (right1->energy > 0)
        right1 = right1->next, step1++;
    }
    customer *left2 = this->Rcurrent, *right2 = this->Rcurrent->next;
    int step2 = 0;
    while (left2 != right2 && step2 < this->tempsize) {
      if (left2->energy > 0 && right2->energy > 0) {
        swap(left2, right2);
        left2 = left2->prev;
        right2 = right2->next;
        step2 += 2;
        continue;
      }
      if (left2->energy < 0)
        left2 = left2->prev, step2++;
      if (right2->energy < 0)
        right2 = right2->next, step2++;
    }
    if (this->Rcurrent->name == nameOfCurr)
      return;
    else {
      while (this->Rcurrent->name != nameOfCurr)
        this->Rcurrent = this->Rcurrent->next;
    }
  }
  // function for REVERSAL method
  void REVERSAL() {
    // cout << "reversal\n";
    if (this->tempsize == 0 || this->tempsize == 1)
      return;
    this->reversal();
  }

  // helper function for UNLIMITED_VOID method
  // Kadane's algorithm
  void printSubsegment(customer *s, customer *e) {
    customer *min = s, *i = s;
    while (i != e) {
      if (i->energy < min->energy)
        min = i;
      i = i->next;
    }
    if (e->energy < min->energy)
      min = e;
    customer *j = min;
    while (j != e) {
      j->print();
      j = j->next;
    }
    j->print();
    customer *k = s;
    while (k != min) {
      k->print();
      k = k->next;
    }
  }
  void findSubsegment() {
    if (this->tempsize < 4)
      return;
    int res_min = 99999999, max_len = 1;
    customer *start, *end, *curr = this->Rcurrent;
    for (int i = 0; i < this->tempsize; i++) {
      int min_ending_here = 0;
      customer *tmp = curr, *s = curr, *head = nullptr, *tail = nullptr;
      for (int j = 0; j < this->tempsize; j++) {
        min_ending_here += tmp->energy;
        if (j >= 3 && (min_ending_here < res_min ||
                       min_ending_here == res_min && j + 1 >= max_len)) {
          res_min = min_ending_here;
          head = s;
          tail = tmp;
          max_len = j + 1;
        }
        tmp = tmp->next;
      }
      if (head && tail)
        start = head, end = tail;
      curr = curr->next;
    }
    this->printSubsegment(start, end);
  }
  // function for UNLIMITED_VOID method
  void UNLIMITED_VOID() {
    // cout << "unlimited_void\n";
    this->findSubsegment();
  }

  // helper function for DOMAIN_EXPANSION method
  // "customers are at the restaurant" means customers are at the table and in
  // the queue
  int sumOfSorcerers() {
    return orderQ->sumOfSorcerers() + oR->sumOfSorcerers();
  }
  int sumOfCurses() { return orderQ->sumOfCurses() + oR->sumOfCurses(); }
  void removeAllSorcerersAndPrint() { // đuổi toàn bộ chú thuật sư
    oR->printAllSorcerers();
    orderQ->printAllSorcerers();
    queue->removeAllSorcerers();
    oR->removeAllSorcerers();
    customer *oh = orderQ->getQhead();
    while (oh) {
      if (oh->energy > 0)
        this->remove(oh->name);
      oh = oh->next;
    }
    orderQ->removeAllSorcerers();
  }
  void removeAllCursesAndPrint() { // đuổi toàn bộ chú linh
    oR->printAllCurses();
    orderQ->printAllCurses();
    queue->removeAllCurses();
    oR->removeAllCurses();
    customer *oh = orderQ->getQhead();
    while (oh) {
      if (oh->energy < 0)
        this->remove(oh->name);
      oh = oh->next;
    }
    orderQ->removeAllCurses();
  }
  // function for DOMAIN_EXPANSION methodd
  void DOMAIN_EXPANSION() {
    // cout << "domain expansion\n";
    if (this->tempsize == 0)
      return;
    if (this->sumOfSorcerers() >= abs(this->sumOfCurses())) {
      this->removeAllCursesAndPrint();
    } else
      this->removeAllSorcerersAndPrint();
    if (!queue->isEmpty()) {
      while (!queue->isEmpty() && this->tempsize < MAXSIZE) {
        customer *newnode = queue->pop();
        queue->dequeue();
        this->RED(newnode->name, newnode->energy);
        customer *tmp = oR->getQhead();
        while (tmp) {
          if (tmp->name == newnode->name) {
            oR->remove(tmp);
            break;
          }
          tmp = tmp->next;
        }
      }
    }
  }

  void LIGHT(int num) {
    // cout << "light " << num << endl;
    if (this->tempsize == 0)
      return;
    if (num > 0) {
      this->Rcurrent->print();
      customer *tmp = this->Rcurrent->next;
      while (tmp && tmp != this->Rcurrent) {
        tmp->print();
        tmp = tmp->next;
      }
    } else if (num == 0) {
      queue->Qprint();
    } else {
      this->Rcurrent->print();
      customer *tmp = this->Rcurrent->prev;
      while (tmp && tmp != this->Rcurrent) {
        tmp->print();
        tmp = tmp->prev;
      }
    }
  }
};