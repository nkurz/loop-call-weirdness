for FUNCTION in  tight variable call write prefetch selfdummy jump dummyread 2mod exchange
do 
    echo -n "$FUNCTION, "
    perf stat -x ", " -e cycles -r 4 loop-call-weirdness $FUNCTION
done