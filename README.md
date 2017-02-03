# forkd
Log forks in userspace, by abusing /proc/loadavg

Proof that one should not place sensitive information on the
command line for any process. Fork safely!

Example output:
```bash
$ ./forkd
23188:  ./forkd
23189:  bash

        ls --color=auto
23190:  sshd: [accepted]

        /usr/sbin/sshd -D -R
23191:  sshd: [accepted]
        sshd: [net]
23192:  -bash
23193:  /usr/bin/hostname

[...]
```
