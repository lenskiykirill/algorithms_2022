for i in $(seq 1 100000); do
      A=$(python3 test.py | ./test 2>&1)
      if [[ ! -z $A ]]; then
            break
      fi
done;
