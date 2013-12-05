// http://eli.thegreenplace.net/2013/12/03/intel-i7-loop-performance-anomaly/

#ifdef IACA
#include </opt/intel/iaca/include/iacaMarks.h>
#else
#define IACA_START
#define IACA_END
#endif

const unsigned N = 400 * 1000 * 1000;
 
volatile unsigned long long counter = 0;
 
void tightloop() {
    IACA_START;
    unsigned j;
    for (j = 0; j < N; ++j) {
        counter += j;
    }
    IACA_END;
}
 

void loop_with_extra_call() {
    IACA_START;
    unsigned j;
    for (j = 0; j < N; ++j) {
        __asm__("call foo");
        counter += j;
    }
    IACA_END;
}

void loop_exchange() {
    IACA_START;
    unsigned j;
    for (j = 0; j < N; ++j) {
        __asm__("xchg %rdx, %rdx"); // assumes counter is %rdx
        counter += j;
    }
    IACA_END;
}

 
void foo() {
    // same performance as foo_full
}

void foo_full() {
    __asm__("movaps %xmm0, %xmm0");
    __asm__("movaps %xmm0, %xmm0");
    __asm__("movaps %xmm0, %xmm0");
    __asm__("movaps %xmm0, %xmm0");
}


void loop_mod_2() {
    IACA_START;
    unsigned j;
    for (j = 0; j < N; ++j) {
        if (j % 2)  __asm__("call foo");
        counter += j;
    }
    IACA_END;
}

volatile unsigned dummy = 0;
void loop_dummy_read() {
    IACA_START;
    unsigned j;
    unsigned dummy_read;
    for (j = 0; j < N; ++j) {
        dummy_read = dummy;
        counter += j;
    }
    IACA_END;
}

void loop_dummy_read_self() {
    IACA_START;
    unsigned j;
    unsigned dummy_read;
    for (j = 0; j < N; ++j) {
        dummy_read = counter; 
        counter += j;
    }
    IACA_END;
}

void loop_dummy_prefetch() {
    IACA_START;
    unsigned j;
    for (j = 0; j < N; ++j) {
        __builtin_prefetch((void *)&dummy);
        counter += j;
    }
    IACA_END;
}

void loop_write() {
    IACA_START;
    unsigned j;
    for (j = 0; j < N; ++j) {
        dummy = j;
        counter += j;
    }
    IACA_END;
}



void loop_jump() {
    IACA_START;
    unsigned j;
    for (j = 0; j < N; ++j) {
        __asm__("test $1, %al");
        __asm__("jnz 1f");
        __asm__("call foo");
        __asm__("1:");
        counter += j;
    }
    IACA_END;
}
 
int main(int argc, char** argv) {
    if (argc <= 1) {
        return 1;
    }
 
    if (argv[1][0] == 't') {
        tightloop();
    } else if (argv[1][0] == 'c') {
        loop_with_extra_call();
    } else if (argv[1][0] == 'e') {
        loop_exchange();
    } else if (argv[1][0] == 'w') {
        loop_write();
    } else if (argv[1][0] == '2') {
        loop_mod_2();
    } else if (argv[1][0] == 'd') {
        loop_dummy_read();
    } else if (argv[1][0] == 's') {
        loop_dummy_read_self();
    } else if (argv[1][0] == 'p') {
        loop_dummy_prefetch();
    } else if (argv[1][0] == 'j') {
        loop_jump();
    }
 
    return 0;
}
