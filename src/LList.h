#ifndef LList_h
#define LList_h

// Need to namespace LinkedList because it's already implemented by
// AsyncWebServer

namespace LL {
    #include <LinkedList.h>
};

// Extend the class that we have a shortcut
template <typename T> class LList : public LL::LinkedList<T> {
    public:
        // because of some crazy occurences i needed to copy this code
        // otherwise the arduino ide will not find it

        void sort(std::function<int(T &, T &)> cmp) {
            if(this->_size < 2) return; // trivial case;

            for(;;) {	
                LL::ListNode<T> **joinPoint = &this->root;

                while(*joinPoint) {
                    LL::ListNode<T> *a = *joinPoint;
                    LL::ListNode<T> *a_end = this->findEndOfSortedString(a, cmp);
            
                    if(!a_end->next	) {
                        if(joinPoint == &this->root) {
                            this->last = a_end;
                            this->isCached = false;
                            return;
                        }
                        else {
                            break;
                        }
                    }

                    LL::ListNode<T> *b = a_end->next;
                    LL::ListNode<T> *b_end = this->findEndOfSortedString(b, cmp);

                    LL::ListNode<T> *tail = b_end->next;

                    a_end->next = NULL;
                    b_end->next = NULL;

                    while(a && b) {
                        if(cmp(a->data, b->data) <= 0) {
                            *joinPoint = a;
                            joinPoint = &a->next;
                            a = a->next;	
                        }
                        else {
                            *joinPoint = b;
                            joinPoint = &b->next;
                            b = b->next;	
                        }
                    }

                    if(a) {
                        *joinPoint = a;
                        while(a->next) a = a->next;
                        a->next = tail;
                        joinPoint = &a->next;
                    }
                    else {
                        *joinPoint = b;
                        while(b->next) b = b->next;
                        b->next = tail;
                        joinPoint = &b->next;
                    }
                }
            }
        }

        LL::ListNode<T>* findEndOfSortedString(LL::ListNode<T> *p, std::function<int(T &, T &)> cmp) {
	        while(p->next && cmp(p->data, p->next->data) <= 0) {
		        p = p->next;
	        }
	
	        return p;
        }
};

#endif