Lab exercises for Compilers 2024/25
===============================================

This repository contains the specifications for the three lab exercises for the Compilers module.

The three exercises are:

1 - [Lexers and regular expressions](1-regexes) (due Fri 31 Jan 2025 at 15:00).

2 - [Parsers and ASTs](2-parsers) (due Fri 07 Feb 2025 at 15:00).

3 - [Code Generation](3-codegen) (due Fri 14 Feb 2025 at 15:00).

Rationale for deadlines: 

- There are two lab sessions (20 Jan and 27 Jan) before the first exercise is due.
- There is one further lab session (3 Feb) before the second exercise is due.
- There is one further lab session (10 Feb) before the third exercise is due.
- There is no assessment related to this module in mid-term exams week, so the labs and lectures will happen as normal.
- The first exercise gets a bonus lab session to give students a bit of extra time to get used to the software environment (Docker, and so on).

The primary purpose of the lab exercises is to ensure that everyone learns the basic skills needed for the main coursework. They are not designed to be _too_ challenging, but they do require you to get to grips with several tools and concepts that you will need later.


Assessment and Feedback
=======================

The three exercises are equally weighted, and are together worth 10% of your Compilers mark (and hence 5% of your overall IAC mark).

The exercises will be marked in a largely automated fashion. I will compile your code and run it against a collection of my own inputs. Please note that it is very important to test that your code compiles and runs correctly. Failure to do so could result a mark of zero for that exercise.

Feedback will be largely in terms of what doesn't work, if anything. I hope to provide feedback within ten working days. You are also encouraged to seek informal feedback on your code from the teaching assistants and from your peers.


Test environment
================

The target environment for the labs (and the coursework) is Ubuntu 22.04. It is strongly suggested that you do your final testing before each submission in this environment, otherwise you are likely to hit incompatibility problems, which may mean your program won't build in my test environment. Those using Mac OS should watch out in particular, as the build environment is often subtly different.

If you want to work on your own machine, you can install a version of Ubuntu
quite easily. For this, we recommend 3 options:
- [Docker](#docker)
- [Vagrant with Virtual Box](#vagrant)
- [Windows Subsystem for Linux (WSL) - Windows Only](#wsl-windows-only)

A guide to each of the installations is found below.

## Docker
**Requirements:**
- [Docker](https://www.docker.com/)

This repository contains a [Dockerfile](Dockerfile), which is a script that sets up a blank Ubuntu 22.04 environment, and then installs all the tools that the lab exercises require. If you configure your testing environment to match the Dockerfile script, you should be fine.

You can enter the virtual machine described by the Dockerfile by going to the directory that contains the Dockerfile and running the following series of commands. First, create the virtual machine "image" by running:

    docker build -t "compilers_labs" .
	
(The `.` at the end is part of the command, by the way. It tells Docker to look in the current directory for the Dockerfile script.) Then create a "container" so that you can enter the virtual machine by running:

    docker run -it -v "`pwd`:`pwd`" -w "`pwd`" --name "compilers_labs_machine" compilers_labs
	
You are now in an Ubuntu 22.04 shell with all the required tools installed. (By the way, the `-it` flag instructs Docker to create a shell through which you can interact with the virtual machine. The `-v pwd:pwd` and `-w pwd` flags mean that your host machine's files are accessible to your virtual machine.) 

When you're finished, you can leave the shell by typing:

    exit
	
If you want to clean up, you can remove the container you created by typing:

	docker ps -a
	docker rm compilers_labs_machine
	
and then removing the image by typing:

	docker images
	docker rmi compilers_labs
	
## Vagrant
**Requirements:**
- [VirtualBox](https://www.virtualbox.org/)
- [Vagrant](https://www.vagrantup.com/)

This repository contains a [Vagrantfile](Vagrantfile), which is a script that sets up a blank Ubuntu environment, and then installs all the tools that the lab exercises require. To start a virtual machine (VM) using Vagrant, follow these steps.

- Open a shell in this directory.

- Run `vagrant up` to download, install, and configure the VM.

- Run `vagrant ssh` to log into the VM.

- Run `cd /vagrant` : This will put you in the same directory, with
  your host files shared with the VM.

You can then use command line build tools in the VM, while editing
the source files in your host editor.

If you want to stop the machine,

- Run `exit` to exit the session,

- Run `vagrant halt` to shut down the virtual machine.

If you later run `vagrant up` again, it will not need to download the VM from scratch.

	
## WSL (Windows only)
- [Windows
Subsystem for Linux (WSL)](https://docs.microsoft.com/en-us/windows/wsl/about)

Check which version of WSL you're using by running `lsb_release -d` and check that it's at least Ubuntu 22.04. Once you're on the correct version, simply ensure you have all the right packages installed by running:
```
sudo apt-get install -y lsb-release g++ gdb make dos2unix git bison flex qemu
```
From here you should be good to go!


Your `git` repository
======================

Setting up your repository
--------------------------

A private repository has been created for you, which you can clone using the following command (but replace `${LOGIN}` with your college login):
````
git clone https://github.com/LangProc/langproc-2024-lab-${LOGIN}.git
````

You will need to use a personal access token (PAT) to authenticate yourself. You can set up a token by following [these instructions](https://docs.github.com/en/authentication/keeping-your-account-and-data-secure/creating-a-personal-access-token). Alternatively, you could clone the repository using SSH, having set up an SSH key by following [these instructions](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent).

Using your repository
---------------------

You are free to include any files you wish in your repository, such as notes. However, try not check in compiled programs or large binary files. Repositories should contains the sources and instructions for binaries, but `git` does not deal well with binary files. Most repositories should contain a [.gitignore](https://git-scm.com/docs/gitignore) file, which gives patterns for files that should not be committed. There is [one included here](2-parsers/.gitignore) which covers a few things, but feel free to add other temporary and binary files that your system might produce.

You are encouraged to commit (and push) to your repository regularly. This is for two reasons:
- If you lose your laptop, you only lose the work you have done since your last push.
- An independently verifiable history of commits allows you to make a strong case that you have not plagiarised your work.

Moreover, when it comes to working in pairs for the coursework, regular commits (and pushes) will help to reduce the severity of merge conflicts.

Synchronising with the specification repository
-----------------------------------------------

If there are any changes to the specification (e.g. bugs or updates), you can incorporate them into your version by pulling again from the specification repo. First you need to make sure it is included as a "remote":
````
git remote add spec https://github.com/LangProc/langproc-2024-lab.git
````
If you now list the remotes, you should see both "origin" (your private repo), and "spec" (the specification repo):
````
git remote -v
````
You can now integrate changes from the specification repo by pulling from it:
````
git pull spec main
````

If you have changed a file that also changed in the specification, then you may need to commit locally first. Look carefully at any incoming changes due to the pull, and make sure you want to accept them.

Submission process
==================

Submission of code is via your `git` repository. Make sure you have committed and pushed to GitHub all the files that your project needs. You are _strongly_ encouraged to clone it into a different directory and then test again, just in case you are relying on something that wasn't committed.

Once your code is committed, note the [commit hash](https://blog.thoughtram.io/git/2014/11/18/the-anatomy-of-a-git-commit.html)
of your current revision:

````
git log -1
````
This will produce something like:
````
ubuntu@ubuntu-xenial:/vagrant$ git log -1
commit 94d8419b20c78da86415bea7236d3719915977a3
Author: John Wickerson <j.wickerson@imperial.ac.uk>
Date:   Mon Jan 07 14:26:40 2018 +0000

    All tests passing.
````

The commit hash of this revision is `94d8419b20c78da86415bea7236d3719915977a3` which is a cryptographic hash over all the files in your repository, including the history of all the files. The hash produced on your local machine will also match the hash calculated by GitHub.

So take your hash (and just the hash), and submit it via Teams.  Even if GitHub goes down, you can later on prove that the existence of your hash in Teams means you must have done the work associated with the hash. The hash in Teams will also be the exact revision of your repository that will get checked out of GitHub and tested. So you can carry on editing and modifying the repository, but only the commit with the hash submitted to Teams will be tested.

To summarise:

1. Test your code on your machine.

2. Commit your code to your local repo.

3. Note the commit hash (`git log -1`).

4. Push the code to GitHub.

5. Check that the commit hash in the GitHub website matches your local hash.

6. **Strongly recommended**: clone your code to a completely different directory, and test it again.

7. Submit the hash via Teams.

You can repeat this process as many times as you want, up until the deadline.

Acknowledgements
================

The exercises were devised by David Thomas (course lecturer until 2017-18), and are nowadays maintained by [John Wickerson](https://johnwickerson.github.io/). I'd like to thank Quentin Corradi, Yann Herklotz, Shuyuan Liu, James Nock, Michalis Pardalos, Simon Staal, and Samuel Wang for suggesting various improvements.
