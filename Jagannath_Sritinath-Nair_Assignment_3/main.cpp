/*
 * Name of Program: EECS 348 Assignment 3 - CEO Email Prioritizer
 * File Name:       main.cpp
 * 
 * Description:
 *     This program prioritizes emails for a company CEO using a custom, list-based 
 *     MaxHeap priority queue implemented from scratch without using <queue> or standard 
 *     heap algorithms. Emails are prioritized first by sender rank (Boss > Subordinate > 
 *     Peer > ImportantPerson > OtherPerson) and then by date (newest date first). 
 *     Identical rank and date ties are resolved via first-in, first-out (FIFO) arrival order.
 * 
 * Inputs:
 *     Standard input (std::cin) commands:
 *       - EMAIL <sender>, <subject>, <date>
 *       - NEXT
 *       - READ
 *       - COUNT
 * 
 * Outputs:
 *      Terminal output displaying the next email, unread count, or confirmation of reading.
 * 
 * Author:          Jagannath Sritinath Nair
 * Creation Date:   October 1, 2026
 * Revision Date:   October 1, 2026
 * Revisions:       Optimized memory with reserve(), added move semantics, completed
 *                  prologue and block documentation.
 * 
 * Other Collaborators: None
 * External Sources:   Claude 3.5 Sonnet (https://claude.ai)
 */

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <utility>

/*
 * Class: Email
 * Description: Represents an email entity storing sender, subject, date, rank, and sequence.
 * Source: Claude 3.5 Sonnet
 * Other Collaborators: None
 */
class Email {
private:
    std::string sender_;   /* Sender category string (e.g., Boss, Peer) */
    std::string subject_;  /* Subject line string */
    std::string date_;     /* Raw MM-DD-YYYY date string for display */
    int senderRank_;       /* Numerical priority rank (5 = Boss down to 1 = OtherPerson) */
    long dateKey_;         /* Integer representation (YYYYMMDD) for fast comparison */
    long sequence_;        /* Sequence arrival counter for FIFO tie-breaking */

    /*
     * Function: rankOf
     * Description: Maps a sender category string to its numerical priority rank.
     * Source: Claude 3.5 Sonnet
     * Other Collaborators: None
     */
    static int rankOf(const std::string& s) {
        if (s == "Boss") return 5;             /* Highest priority category */
        if (s == "Subordinate") return 4;      /* Second priority category */
        if (s == "Peer") return 3;             /* Third priority category */
        if (s == "ImportantPerson") return 2;  /* Fourth priority category */
        return 1;                              /* Lowest priority category (OtherPerson) */
    }

    /*
     * Function: keyOf
     * Description: Parses MM-DD-YYYY string into a YYYYMMDD integer for comparisons.
     * Source: Claude 3.5 Sonnet
     * Other Collaborators: None
     */
    static long keyOf(const std::string& d) {
        int mm = 0, dd = 0, yyyy = 0;          /* Variables to hold month, day, and year */
        char a = 0, b = 0;                     /* Delimiter variables to absorb dash separators */
        std::istringstream in(d);              /* Stream to parse input date string */
        in >> mm >> a >> dd >> b >> yyyy;      /* Extract month, dash, day, dash, year */
        return static_cast<long>(yyyy) * 10000L + mm * 100L + dd; /* Convert to YYYYMMDD */
    }

public:
    /*
     * Constructors
     * Source: Claude 3.5 Sonnet
     * Other Collaborators: None
     */
    Email() : senderRank_(0), dateKey_(0), sequence_(0) {} /* Default constructor */

    Email(const std::string& sender, const std::string& subject,
          const std::string& date, long sequence)
        : sender_(sender), subject_(subject), date_(date),
          senderRank_(rankOf(sender)), dateKey_(keyOf(date)), sequence_(sequence) {} /* Parameterized constructor */

    /*
     * Getters
     * Source: Claude 3.5 Sonnet
     * Other Collaborators: None
     */
    const std::string& getSender() const { return sender_; }   /* Returns sender category string */
    const std::string& getSubject() const { return subject_; } /* Returns subject line string */
    const std::string& getDate() const { return date_; }       /* Returns date string */

    /*
     * Function: hasHigherPriorityThan
     * Description: Priority comparison between two emails using rank, date, and FIFO sequence.
     * Source: Claude 3.5 Sonnet
     * Other Collaborators: None
     */
    bool hasHigherPriorityThan(const Email& other) const {
        if (senderRank_ != other.senderRank_) return senderRank_ > other.senderRank_; /* Compare sender priority rank */
        if (dateKey_ != other.dateKey_) return dateKey_ > other.dateKey_;            /* Compare date (newer date wins) */
        return sequence_ < other.sequence_;                                          /* FIFO tie-breaker (earlier arrival wins) */
    }
};

/*
 * Class: MaxHeap
 * Description: List-backed dynamic array MaxHeap priority queue implementation.
 * Source: Claude 3.5 Sonnet
 * Other Collaborators: None
 */
class MaxHeap {
private:
    std::vector<Email> data_; /* Dynamic array storage container for heap elements */

    /* Helper functions for binary heap parent/child index arithmetic */
    static std::size_t parent(std::size_t i) { return (i - 1) / 2; } /* Calculates parent index */
    static std::size_t left(std::size_t i) { return 2 * i + 1; }     /* Calculates left child index */
    static std::size_t right(std::size_t i) { return 2 * i + 2; }    /* Calculates right child index */

    /*
     * Function: swapAt
     * Description: Swaps elements at indices i and j using std::swap.
     * Source: Claude 3.5 Sonnet
     * Other Collaborators: None
     */
    void swapAt(std::size_t i, std::size_t j) {
        std::swap(data_[i], data_[j]); /* Swaps two elements in-place inside vector */
    }

    /*
     * Function: siftUp
     * Description: Restores MaxHeap invariant upwards from given index.
     * Source: Claude 3.5 Sonnet
     * Other Collaborators: None
     */
    void siftUp(std::size_t i) {
        while (i > 0) {                                            /* Continue while current node is not root */
            std::size_t p = parent(i);                             /* Fetch parent index */
            if (data_[i].hasHigherPriorityThan(data_[p])) {        /* Check if child has higher priority than parent */
                swapAt(i, p);                                      /* Swap child and parent */
                i = p;                                             /* Move index up to parent position */
            } else {
                break;                                             /* Heap property is restored */
            }
        }
    }

    /*
     * Function: siftDown
     * Description: Restores MaxHeap invariant downwards from given index.
     * Source: Claude 3.5 Sonnet
     * Other Collaborators: None
     */
    void siftDown(std::size_t i) {
        const std::size_t n = data_.size();                        /* Store current size of heap array */
        while (true) {
            std::size_t l = left(i), r = right(i), best = i;      /* Initialize left, right, and best candidate indices */
            if (l < n && data_[l].hasHigherPriorityThan(data_[best])) best = l; /* Check left child priority */
            if (r < n && data_[r].hasHigherPriorityThan(data_[best])) best = r; /* Check right child priority */
            if (best == i) break;                                  /* Node is larger than both children; stop */
            swapAt(i, best);                                       /* Swap with higher-priority child */
            i = best;                                              /* Move index down to child position */
        }
    }

public:
    /*
     * Constructor
     * Description: Pre-allocates initial dynamic memory capacity for optimization.
     * Source: Claude 3.5 Sonnet
     * Other Collaborators: None
     */
    MaxHeap() {
        data_.reserve(128); /* Pre-allocate vector capacity to avoid initial reallocation overhead */
    }

    bool isEmpty() const { return data_.empty(); } /* Returns true if heap contains zero items */
    std::size_t size() const { return data_.size(); } /* Returns current element count */

    /*
     * Function: insert
     * Description: Inserts an email into the priority queue and sifts up.
     * Source: Claude 3.5 Sonnet
     * Other Collaborators: None
     */
    void insert(Email e) {
        data_.push_back(std::move(e)); /* Move email into vector end to prevent unnecessary copies */
        siftUp(data_.size() - 1);       /* Bubble new node up to proper position */
    }

    /*
     * Function: peek
     * Description: Returns reference to highest priority email at heap root.
     * Source: Claude 3.5 Sonnet
     * Other Collaborators: None
     */
    const Email& peek() const { return data_[0]; } /* Returns root node reference */

    /*
     * Function: removeTop
     * Description: Removes root element from priority queue and restores heap property.
     * Source: Claude 3.5 Sonnet
     * Other Collaborators: None
     */
    bool removeTop() {
        if (data_.empty()) return false;          /* Return false if heap is already empty */
        data_[0] = std::move(data_.back());        /* Move last element to root position */
        data_.pop_back();                          /* Remove last element entry */
        if (!data_.empty()) siftDown(0);           /* Rebalance heap downwards from root */
        return true;                               /* Return true on successful removal */
    }
};

/*
 * Class: InboxManager
 * Description: Manages user input commands, line trimming, and heap operation triggers.
 * Source: Claude 3.5 Sonnet
 * Other Collaborators: None
 */
class InboxManager {
private:
    MaxHeap heap_;          /* Priority queue storage container instance */
    long nextSequence_;     /* Auto-incrementing arrival sequence counter */

    /*
     * Function: trim
     * Description: Strips leading and trailing whitespace, tabs, and carriage returns.
     * Source: Claude 3.5 Sonnet
     * Other Collaborators: None
     */
    static std::string trim(const std::string& s) {
        const char* ws = " \t\r\n";                        /* Whitespace characters set */
        std::size_t b = s.find_first_not_of(ws);           /* Find first non-whitespace character index */
        if (b == std::string::npos) return "";             /* Return empty string if all whitespace */
        std::size_t e = s.find_last_not_of(ws);            /* Find last non-whitespace character index */
        return s.substr(b, e - b + 1);                     /* Return trimmed substring */
    }

    /*
     * Function: handleEmail
     * Description: Parses EMAIL command string and inserts extracted email into MaxHeap.
     * Source: Claude 3.5 Sonnet
     * Other Collaborators: None
     */
    void handleEmail(const std::string& args) {
        std::size_t first = args.find(',');                /* Find comma separating sender from subject */
        std::size_t last = args.rfind(',');                /* Find comma separating subject from date */
        if (first == std::string::npos || last == first) return; /* Guard against malformed line inputs */
        
        std::string sender = trim(args.substr(0, first));                /* Extract sender string */
        std::string subject = trim(args.substr(first + 1, last - first - 1)); /* Extract subject line */
        std::string date = trim(args.substr(last + 1));                  /* Extract date string */
        
        heap_.insert(Email(sender, subject, date, nextSequence_++));    /* Insert new Email into heap */
    }

    /*
     * Function: handleNext
     * Description: Displays highest priority email in queue without removing it.
     * Source: Claude 3.5 Sonnet
     * Other Collaborators: None
     */
    void handleNext() const {
        if (heap_.isEmpty()) return;                       /* Guard against empty queue access */
        const Email& e = heap_.peek();                     /* Peek top email without popping */
        std::cout << "Next email:\n"                       /* Print expected NEXT prefix line */
                  << "Sender: " << e.getSender() << "\n"   /* Print sender category */
                  << "Subject: " << e.getSubject() << "\n" /* Print subject line */
                  << "Date: " << e.getDate() << "\n";      /* Print date string */
    }

    /*
     * Function: handleRead
     * Description: Silently removes highest priority email from queue.
     * Source: Claude 3.5 Sonnet
     * Other Collaborators: None
     */
    void handleRead() { 
        heap_.removeTop();                                 /* Pop top element silently from heap */
    }

    /*
     * Function: handleCount
     * Description: Prints current number of unread emails to standard output.
     * Source: Claude 3.5 Sonnet
     * Other Collaborators: None
     */
    void handleCount() const {
        std::cout << "There are " << heap_.size() << " emails to read.\n"; /* Print unread count */
    }

public:
    /*
     * Constructor
     * Source: Claude 3.5 Sonnet
     * Other Collaborators: None
     */
    InboxManager() : nextSequence_(0) {}                   /* Initialize sequence counter to 0 */

    /*
     * Function: processLine
     * Description: Identifies command token and delegates to handling functions.
     * Source: Claude 3.5 Sonnet
     * Other Collaborators: None
     */
    void processLine(const std::string& rawLine) {
        std::string line = trim(rawLine);                  /* Trim raw input line */
        if (line.empty()) return;                          /* Ignore blank lines */

        std::size_t sp = line.find_first_of(" \t");        /* Find delimiter space after command name */
        std::string cmd = (sp == std::string::npos) ? line : line.substr(0, sp); /* Extract command token */
        std::string args = (sp == std::string::npos) ? "" : trim(line.substr(sp + 1)); /* Extract arguments */

        if (cmd == "EMAIL") handleEmail(args);             /* Handle EMAIL insertion command */
        else if (cmd == "NEXT") handleNext();               /* Handle NEXT print command */
        else if (cmd == "READ") handleRead();               /* Handle READ removal command */
        else if (cmd == "COUNT") handleCount();             /* Handle COUNT tally command */
    }

    /*
     * Function: run
     * Description: Reads command stream line-by-line until end-of-file (EOF).
     * Authored By: Claude 3.5 Sonnet
     * Other Collaborators: None
     */
    void run(std::istream& in) {
        std::string line;                                  /* String variable for raw input lines */
        while (std::getline(in, line)) processLine(line);  /* Read input stream until EOF */
    }
};

/*
 * Function: main
 * Description: Program entry point. Instantiate InboxManager and start command loop.
 * Source: Claude 3.5 Sonnet
 * Other Collaborators: None
 */
int main() {
    InboxManager manager;                                  /* Instantiate inbox manager object */
    manager.run(std::cin);                                 /* Run input processing loop on standard input */
    return 0;                                              /* Return 0 indicating successful execution */
}
