//
// Created by worker on 2020/6/19.
//
#include "port_stdcxx.h"

using leveldb::port::Mutex;

class BankAccount {
 private:
  Mutex mu;
  //
  int   balance GUARDED_BY(mu);

  void depositImpl(int amount) {
    mu.Lock();
    balance += amount;       // WARNING! Cannot write balance without locking mu.
    mu.Unlock();
  }

  void withdrawImpl(int amount) {
    mu.Lock();
    balance -= amount;       // OK. Caller must have locked mu.
  }

 public:
  void withdraw(int amount) {
    mu.Lock();
    withdrawImpl(amount);    // OK.  We've locked mu.
  }                          // WARNING!  Failed to unlock mu.

  void transferFrom(BankAccount& b, int amount) {
    mu.Lock();
    b.withdrawImpl(amount);  // WARNING!  Calling withdrawImpl() requires locking b.mu.
    depositImpl(amount);     // OK.  depositImpl() has no requirements.
    mu.Unlock();
  }
};

int main() {
  BankAccount account;
}
