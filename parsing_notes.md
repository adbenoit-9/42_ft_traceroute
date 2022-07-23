# notes from vm
-w MAX,HERE,NEAR  --wait=MAX,HERE,NEAR
                              Wait for a probe no more than HERE (default 3)
                              times longer than a response from the same hop,
                              or no more than NEAR (default 10) times than some
                              next hop, or MAX (default 5.0) seconds (float
                              point values allowed too)

## Tests
```
$ traceroute -tm 2 8.8.8.8
Cannot handle `-t' option with arg `m' (argc 1)
$ traceroute -t 8.8.8.8  
Cannot handle `-t' option with arg `8.8.8.8' (argc 2)
$ traceroute - 8.8.8.8 
-: Name or service not known
Cannot handle "host" cmdline arg `-' on position 1 (argc 1)
$ traceroute -arg 8.8.8.8
Bad option `-a' (argc 1)
$ traceroute  8.8.8.8 -f 2 => good syntax
$ traceroute  8.8.8.8 --hel 
Bad option `--hel' (argc 2)
$ traceroute  8.8.8.8 123377777
too big packetlen 123377777 specified
$ traceroute  8.8.8.8 1        
traceroute to 8.8.8.8 (8.8.8.8), 30 hops max, 28 byte packets
$ traceroute  8.8.8.8 -1
Bad option `-1' (argc 2)
$ 8.8.8.: Name or service not known
Cannot handle "host" cmdline arg `8.8.8.' on position 1 (argc 1)
$ traceroute  -f 65 localhost
first hop out of range
$ traceroute  -f -1 localhost
first hop out of range
$ traceroute  -m -1 localhost
max hops cannot be more than 255
$ traceroute  -q -1 localhost
no more than 10 probes per hop
$ traceroute  -q 0 localhost
no more than 10 probes per hop
$ traceroute  -m 0 localhost
first hop out of range
$ traceroute  -f 0 localhost
first hop out of range
$ traceroute -1               
Bad option `-1' (argc 1)
```

packetlen : min = 28, max = 65000
