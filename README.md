# FT_TRACEROUTE (@42Paris)
*Recoding the traceroute command.*

## Usage

```
$ git clone https://github.com/adbenoit-9/42_ft_traceroute.git
$ cd 42_ft_traceroute
$ make
$ ./ft_traceroute [options] host [packetlen]
```

### Description

<p style="margin: 0px; margin-top: 1em; margin-bottom: 1.5em;margin-left: 1em">
Print the route packets trace to network host
</p>

### Arguments
<p style="margin-top: 1em;">
    <strong>host</strong><br>
    The host to traceroute to<br><br>
    <strong>packetlen</strong><br>
    The full packet length (default is the length of an IP header plus 40). Can be ignored or increased to a minimal allowed value.<br><br>

### Options

<p style="margin-top: 1em;">
    <strong>-f</strong> <ins>first_ttl</ins>  <strong>--first</strong>=<ins>first_ttl</ins></strong><br>
    Start from the first_ttl hop (instead from 1).<br><br>
    <strong>-m</strong> <ins>max_ttl</ins>  <strong>--max-hops</strong>=<ins>max_ttl</ins></strong><br>
    Set the max number of hops (max TTL to be reached). Default is 30.<br><br>
    <strong>-q</strong> <ins>nqueries</ins>  <strong>--queries</strong>=<ins>nqueries</ins><br>
    Set the number of probes per each hop. Default is 3.<br><br>
    <strong>--help</strong><br>
    Print help info and exit.<br><br>
</p>
