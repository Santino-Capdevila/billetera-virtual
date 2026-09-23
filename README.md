# Billetera Virtual — UNIPAGO

A virtual wallet written in C, built as a university project for
*Programación y Metodología de la Investigación*, Universidad Nacional de
San Luis.

The program manages a single account holder's wallet: a contact book, a
transaction log kept sorted by date, and a running balance that every
operation updates.

## Data structures

The point of the exercise is the two structures behind it, both implemented
from scratch with a cursor-based interface (`reset`, `forward`, `copy`,
`insert`, `isOos`, `isEmpty`, `isFull`):

- **Contacts — sequential list.** A fixed array of up to 100 entries with a
  cursor and an `ultimo` index. Inserting shifts the tail to the right,
  deleting shifts it back to the left.
- **Transactions — linked list.** Nodes holding `acc`, `cur` and `aux`
  pointers. New transactions are inserted **in order, most recent first**, by
  walking the list until a less recent date is found.

Two of the queries are solved recursively rather than with a loop: listing the
transactions above $350,000, and counting how many transfers were sent to a
given contact.

## Model

A **contact** holds a name, a CBU or alias, and an account type (savings in
ARS, checking in ARS, or virtual wallet).

A **transaction** holds an auto-incrementing id, the source and destination
account, an operation type (debit or credit), a movement type (transfer, QR
payment or service payment), the amount, a free-text note, a date, and a
status (completed or voided).

The balance moves with each transaction: debits subtract, credits add. Voiding
a transaction reverses its effect instead of deleting it, so the history stays
intact.

## Features

**Contacts**
- List, add and delete contacts

**Transactions**
- Create a transaction, validating the amount against the available balance and a per-operation ceiling
- List every transaction, or just the last 10
- Look up a single transaction by id
- Deposit or withdraw cash
- Edit a transaction's note, either by id or by picking a contact
- Void a transaction, which reverses its effect on the balance
- List and purge voided transactions
- Show every transaction above $350,000 *(recursive)*
- Count the transfers sent to a given alias *(recursive)*
- Export a date range to `historicos.txt`
- Monthly summary: total credited and total debited

Dates are restricted to the October 10 – December 31 window the exercise
defines, and validated on entry.

## Data files

Read at startup:

- `contactos.txt` — preloaded contacts, one per line: `name,alias,type`
- `movimientos.txt` — preloaded transactions, one per line, comma separated

Written by the program:

- `anulados.txt` — appended to whenever a transaction is voided
- `historicos.txt` — overwritten by the date-range export

## Build and run

The sources live in `PMI_ultima_version/_/`. Compile and run from that
directory so the program finds its `.txt` files:

```
cd PMI_ultima_version/_
gcc -o billetera main.c
./billetera
```

With Code::Blocks, open `Ultimo.cbp` instead.

## Author

Santino Capdevila — Computer Science Engineering, UNSL
