# Introduction.

I don’t know how this talk will go over, but I hope that what you take away from this talk is something very different to what you were expecting when you joined. So, let’s go...

---

# Introduction
## How I'll do this talk

There's a lot of technology and features here, which I'll call out as I come to them. Some of what I’m talking about may not be familiar to RISC OS users, but may be much more common to users outside of RISC OS. If there's something you don't understand, and you feel you need clarification, ask a question in the chat if you can. Someone else might be able to answer, or I may be able to address it immediately. At the end of each section I'm going to have a short break to answer specific questions on the section. This should give us a chance to talk on that section’s topics without us getting too far ahead, and people being completely lost.

At the end of the talk, these slides and a bunch of links to resources will be made available.

---

# Introduction
## What we'll talk about

There are 5 sections to the talk -

- a little bit of background to set the scene
- a discussion of what JFPatch-as-a-service is and what you can do with it
- a very light dive into how that service works,
- a look at the system that powers everything.
- and finally some conclusions.

FIXME: Decide if I’m doing demos as I go, or just at the end.

At the end of the talk I’ll take questions on the subjects that have been covered here, for as long as people want to stick around.


---

# Background

We jump back about a year and a half, which will set the scene...

---

# Background
## Who am I?

You might know me as Justin Fletcher, or gerph. I changed my name a few years ago, but I use gerph where I want some continuity.

I'm a RISC OS architect, and in my day job, I'm a software engineer. I've worked with all of RISC OS from chip initialisation through to the applications, and I'm pretty confident I know it better than most, even after 15 years away. Plus, as I will talk about, I’ve worked on many of the RISC OS interfaces more recently, which makes them more familiar.

If you want to know more about my past, the RISC OS Rambles on my website talk about it in much more detail.

---

# Background
## Dear gosh why?

I was found myself being very unhappy. I didn’t have anything that focused me. I didn’t work on RISC OS things because I get angry and upset. And if I’m angry and upset I’m useless for days.

I’ve tried to deal with that, but things still set me off. Any association can be a problem for me - I wrote the Rambles to try to be a cathartic release, but it wasn’t completely successful. So I stay away from the RISC OS world.

I have regular counselling, and I believe that it’s been working - because I realised, about a year and a half ago, that there was something I could do.

I realised that I enjoy RISC OS, but I don’t enjoy the RISC OS community. I’m not intending to offend anyone here by that, but that’s what’s debilitating for me. Stopping myself from doing things with RISC OS, because I understand how badly it affects me, has been a way of coping. But I realised that I can do RISC OS things for myself, and not care about whether anyone else sees them. Just doing it because I enjoy it.

Well, wow. This was lightbulb moment.

That’s why I’m doing this talk - because I’m really proud of that realisation. I can do things because I enjoy them, not because I feel I have to prove things to people.
Or to quell any criticism.
Or because RISC OS desperately needs things that I can do.

To hell with those things - yes, those things are still there, but I’m keeping them under control - because what matters is that I can stay sane and be happy.

That’s what this talk is. It’s my way of saying "this is what, most nights, makes laugh when I go to bed, chuckling ‘this is f’ing nuts’, ‘look how cute that is’. JFPatch-as-a-service is the bit I've made available to the world, but the rest ... we’ll get to that.


---

# Background
## So you want to use RISC OS but...

Let's have a look at what I wanted to get out of this - other than having a project that made me happy.

- RISC OS development tools are a bit poor, and they only run on RISC OS. I don't have a RISC OS system.
Not only that, but if you've worked with DDT, the debugger, you'll know how much like russian roulette that is - if you see the insides of it you'll find out why.

- Projects on RISC OS have awful testing in my experience. That includes my own. Probably the best tested project, in terms of proper test cases, was the SVG converter that I wrote, and that was largely because it had tests from W3C to compare its behaviour against.

Ad-hoc testing is the way to that things are done and the cringeworthy mantra of 'it's compiled? ship it!' seems to apply. Maybe that's not true for developers these days, but that's certainly my impression.

- And RISC OS itself is not a good system for running tests on. It's cooperative, and generally in testing the 'System Under Test' does not cooperate - it will hang, crash, and leak memory like they're going out of style. And on RISC OS that commonly means its time to reboot.

Plus there's no isolation, so even if it doesn't look like the system is broken, it might have corrupted something important.


---

# Background
# How does the real world do things?

Ok, so we've understood some problems; let's see how the real world does things.

These are just a few of the things that out in the real world you expect to have, which I'm going to look at.

Let's look at each of these and how I addressed them.

---

# Background
## How can I do this? (1)

My own source had been in CVS for many years, and in about 2016 I moved everything - even my own ancient RISC OS projects - into git. I have large numbers of python and perl projects managed in git, because it's so much easier to work with than CVS, and even for that one little script you're just tinkering with, you can just create a source repository for it.

Git is a source control system that is intended for large scale distributed use, but which works even for a single user working with a single file. It doesn't need a server, but if you want to share code that's one way you can do it. GitHub is one of the best known systems for storing and accessing things, which people will almost certainly have heard of. However, at the time GitHub wasn't so friendly to private projects, and even still, I didn't want my projects going off site. So I chose to use GitLab - it's installed on my server, and... it's pretty great. Oh, and it's free.

<step>

I was amused - but not at all surprised - that ROOL selected GitLab as their server system, a few years ago.

---

# Background
## How can I do this? (2)

So I've got a way of storing and managing the source, but I want to at least be able to compile and assemble things if I'm going to work on them. It's all well and good being able to build RISC OS components for testing on macOS by stubbing libraries, but I want to be able to build things that will run on RISC OS.

<step>

Back in 2005 I had cross-compiling working just fine for RISC OS and most of the last version of RISC OS that I worked on was actually built on Linux, but it was all 32bit only. So I ported the toolchain to work in 64bit compilers.

It wasn't actually 'simple', and I still see bugs now and then for cases I've missed. The authors loved their bitfields and packing structures into different types. But with a little work, the toolchain was made to compile RISC OS components on macOS.

All the cross-compiling tools have the prefix 'riscos hyphen' to make it easier to distinguish them, so you can see that 'riscos-amu', 'riscos-objasm', and 'riscos-link' were invoked in this example.

I needed to test the C compiler with some nice example code to see that it fared well. I had a lot of my own, but using random stuff from other people to throw at the testing throws up loads of problems you wouldn't otherwise find.

<step>

So I wrote a small library that downloaded all the C code from the Rosetta Code website. If you don't know Rosetta Code, it's a website containing a solutions for many problems, in many different programming languages.

And having downloaded all the programs, I ran them through the C compiler to see whether it crashed or not.

---

# Background
## How can I do this? \(3\)

So the tools for building exist, and I have headers and libraries that I can use to link against.
But I need to get them to my machine.

Traditionally you downloaded and installed a package that gave you the build tools and you used them until the next time you needed to upgrade. In modern development, these tools and libraries change rapidly, and even if they don't you want more control over what exactly went in to building your latest product.

This is usually managed through a software repository that holds all those components that you might want - your toolchain, libraries, documentation, even full releases, can be stored in the repository and retrieved in managed ways. In this way what you're using is reproducible, because everything that went into making it is known, and can be used again.

<step>

I chose Artifactory. It's not great. It's not terrible. It does the job. And it's free.
And I created some small scripts that let me push and pull my RISC OS components in a structured way.

---
# Background
## How can I do this? \(4\)

Sometimes I don't want to download the toolchain and set up binaries. I just want to compile some code. Normally you would just install the toolchain and your libraries on the system in that case. Then they'd be there when you needed them.

Ok, you can do that. But modern build systems have been taking advantage of 'containers'. If you've not come across containers or one of the implementation systems - 'docker' - before, then I'll try to summarise in a couple of sentences.

Containers are a way of bundling all the resources you need - and only those that you need - to do a task into a single package and let you run just that package. Docker allows this to be done within Linux systems, by packaging just the bits of the OS and the tool that you need, and running that in isolation. If you're thinking that's kinda like a RISC OS application that included the modules it wanted, it's a bit like that, but much more - if the application contained the bits of the OS and the boot or system resources, you're getting closer.

<step>

And that's the technology used here - it bundles the RISC OS development tools into a docker container, then lets me build my source with those tools. You can see the docker command is a little unweildy. I actually have a small script called 'riscos-build' which contains the prefix, so I don't run the full command except when demonstrating it.

---

# Background
## How can I do this? \(5\)

The next feature of modern development on my list was automated testing. This is very closely associated with the process of 'Continuous Integration' and the terms are often used interchangeably. The distinction to draw here is between ad-hoc testing, where you test what you think is going to be a problem, and automated testing, where you define tests which are run automatically.

- In ad-hoc testing, you essentially sign off that you think it's good enough.
- In automated testing, your tests tell you whether it's good enough.

Because the tests are automatically run, you find out quickly that things have broken, and you don't reintroduce earlier bugs. Specifically that's feature and regression testing - there are other types of tests that can be run automatically, and you can write your tests to do whatever you want.

<step>

The GitLab source control system has a CI system built in, which lets you run whatever tests you need to. When a change is pushed to the GitLab server, the tests are triggered.

In my case, these tests will:

- download the toolchain and environment from Artifactory
- build the code
- and then push the built result back up to Artifactory if it was successful.

If it wasn't successful, I'll get an email, and the website reports the build log.

---
# Background
## How can I do this? \(6\)

If I'm building, let's say, a RISC OS module, it'd be normal to write all the code in little components that you can try out separately, before bundling them together into the scary SVC enviroment. For a lot of code, you can test those components without needing the rest of the system. And for a lot of code, you can do that on non-RISC OS systems.

If you're just managing data structures, you don't need a real RISC OS system to check that the logic works right, for example. So in those cases you can make your code compile and test on a different platform. That's one way of doing things - and it's very effective.

<step>

But you really want to run things on RISC OS, because that scary SVC environment is a bit different, and you'll want to communicate with other parts of the system in situ.

We'll come to how you do that later.

---
# Background
## How can I do this? \(7\)

My final feature of modern development was to have a fleet of machines available to do your bidding - whether it be testing or processing. And maybe you don't need that, but if you're wanting to run a few thousand tests at a time then maybe you can save time by spreading them over multiple machines.

JFPatch-as-a-service isn't quite that.

But it's a step along the way.

Before I move on to talk about JFPatch-as-a-service itself, let's see if there any questions.

---
# 2. JFPatch-as-a-service

Spin forward a year - it's okay we'll come back to the intervening period in section 4.

---

# JFPatch-as-a-Service
## Why?

In early March, David Thomas semi-challenged me to make some of the things I'd been doing public. I was still nervous about that, and wasn't really comfortable making that kind of announcement.

But the idea of doing something for April 1st, which wasn't actually an April fool, was appealing. I genuinely expected the response to be a 'meh', but that wasn't the point. The whole point of the work I was doing was to be enjoyable - and I like solving problems, particularly ones that are strange. It'd been floating around in my head for a while to do 'something' like it, but it hadn't really solidified until then.

Running an on demand RISC OS build process on a cloud system as a service is probably unique. I've been away and I don't pay attention to the comings and goings, so maybe it's completely passe, but I liked the idea.

And I like the idea of surprising people with something they didn't expect.

---
# JFPatch-as-a-Service
## What is JFPatch?

JFPatch... it's an assembler I wrote when I was at school... something like 25 years ago.
I learnt assembler by reading other people's code and patching it to do different things. Largely that was by writing a BASIC program that loaded the code, assembled some replacement code and saved it again.

That process is tedious. I wanted a shorthand for doing that boilerplate work, so I created a pre-processor that built those BASIC programs from a 'patch file'. I knew I shouldn't pollute the namespace, so the tool got a prefix of my initials.

It's actually not that clever, but it can construct a lot of the boilerplate that goes into writing modules, too. If you want a module that registers itself with a few vectors you can do that in just a couple of lines of code. It even goes so far to let you perform the whole filesystem registration process through some simple definitions in the header.

---

# JFPatch-as-a-Service
## What is the service?

The service is a front end that lets you give it JFPatch code, builds it on a cloud serverm, and then returns the result to you.

I wanted it to be a tool that could be used like Matthew Godbolt's compiler explorer. It does a different thing, and JFPatch-as-a-service isn't anywhere near as polished and professional as that, but... that's where the idea and structure came from. Matthew's an ex-RISC OS user.

---
# JFPatch-as-a-Service
## What can you build with the service? (1)

But what can the service do? Well, it can build any JFPatch code you throw at it, and now the module header is 32bit safe as well. I was too lazy to look at the 32bit header on the modules until recently. Not because I didn't care, but because during the time I might have done so, I was focused on the rest of RISC OS. A niche tool that only I used wasn't really a priority.

But, the service can also build C, and Pascal and run BASIC programs, and build ObjAsm.
If you give it a zip archive containing an AMU Makefile, it would invoke it and return the binary it thought was produced by it.

You can try this out on the real site if you'd like. Here's what you see...

---
# JFPatch-as-a-Service
## What can you build with the service? (3)

That's a C file that I wrote, and then sent to the service, and you see that in the output from the process that it has built and the result was returned as an Absolute file.

---
# JFPatch-as-a-Service
## What can you build with the service? (4)

A very simple Perl program. There's no returned binary from this, because we just printed a message.

---
# JFPatch-as-a-Service
## What can you build with the service? (5)

And finally a BBC BASIC program that also does very little.

As I mentioned, it did all this from April 1st. I had kinda hoped that someone would play with it and find that it did more things. I know I would have - and it's not hidden, the source to the page says explicitly that it supports these things, and describes in one of the files how the languages are recognised.

Nobody came to talk to me about it, so that 'easter egg' remained hidden.

---
# JFPatch-as-a-Service
## What might use the service?

I’ve converted a few things to be able to automatically build on changes. They’re on github with the label `riscos-ci`.

Some of the tools were very simple to make work - Rick Murray's ErrorCancel for example was just a simple matter of adding the right build commands to the repository. Others were a little more involved, like Julie Stamp's CObey module, which I had to tweak the code of, because I'm still using the C89-style compiler.

Running a build automatically on submitting the code is a great way of checking that it still work - presumably you've built it yourself, but it never hurts to have an automated system build things.
It is also possible to do a 'release' of the code automatically in GitHub, so you can actually get a distributable archive out from the build.

If you check the DDEUtilsJF link, you'll see that there are 'releases' listed on the right of the page - those were automatically created after push. They just have to be approved by clicking a button in GitHub.

---
# JFPatch-as-a-Service
## How do you use the service? \(1\)

The service can be used 3 ways - either through the web interface, as you've seen, or by using the API. A service isn't really a service unless it can be used programatically.

There are two APIs for accessing the service - either a JSON HTTP API, or a WebSockets API.

The JSON API is more limited because you basically just post data to it and get back a response. It's also got a timeout of about a minute, which for some jobs may not be long enough.

The WebSockets API however, is an interactive system. If you're not familiar with WebSockets, then that's not especially surprising. They've been around since 2010, but unless you've been working on heavily interactive sites you probably wouldn't encounter them. Unlike the regular HTTP request-response protocol, WebSockets allow packetised transactions. That means you can send a message to the other end, and you can receive them. Packets are always delivered in order, and always delivered completely. Their content is application defined, so once established you can send anything you want.

---
# JFPatch-as-a-Service
## How do you use the JSON API?

Just use a URL library. The URLFetchers on RISC OS would do just as well, but probably you wouldn't invoke a cloud-based RISC OS build client from a RISC OS system. But you could.

And of course, you can parse the JSON response with any JSON library you'd like.

---
# JFPatch-as-a-Service
## How do you use the WebSockets API?

The WebSockets system is a little harder to work with - you definitely need a client to talk to it. Fortunately, in the JFPatch examples repository I supplied python a client that uses a package which does the work for you.

But that's not always a suitable solution - it means you have to install other Python packages,
and you have to have Python in the first place.

---

# JFPatch-as-a-Service
## What is the robuild-client?

The `robuild-client` tool is a little tool that's able to take a file, and give it to the service over the WebSockets interface, and report the output as it is produced by the build process. It can then save the output to a known location, or report an error through the exit status of the command in the normal way. This makes it easy to work into a build pipeline.

The Github project builds a Linux version of the tool...

<step>

... and then using that tool it submits the code to the service, so that we get a RISC OS build out. Why would you want to have run it from RISC OS? I don't know, but completeness says if it's a tool for RISC OS, maybe it should run on RISC OS. More specifically, I can't completely repress that feeling that people in the RISC OS world reject anything that isn't for RISC OS as irrelevant, so I have to make it work for RISC OS too.

I used two libraries to do this - a JSON parser, and a WebSockets library. The JSON parser was originally written by David Gamble, a friend who used to do RISC OS things, although I didn't discover this until afterward.

The repository is also an example of producing releases from an automated build, as well as building RISC OS code from a non-RISC OS code base using `sed` to fix incompatible things.

I didn't say it was a *good* solution!

---

# JFPatch-as-a-Service
## How does the service know what to do?

For simple files, it spots the language from its format. For example, if it's got BASIC tokens in then it's a BASIC file. If it has C comments in, it's a C file. The heuristic is pretty poor, but it works on pretty much anything but simple cases.

Zip archives can contain multiple files, and the services picks out ones that it can understand. If there's a Makefile in the file that it recognises, it'll invoke AMU on the file. If there's a JFPatch file, it'll run that file - which allows you to patch other files.

If the Zip archive contains a `.robuild.yaml` file, this will be used to control how the build runs.

<step>

YAML stands for 'Yet Another Markup Language'. It's nicer to work with than JSON, so long as you avoid the many crazy cases it has. It's pretty nutty in extremis, but it's quite readable.

Although I say the file is called `.robuild.yaml` on RISC OS that would be `/robuild/yaml`, and it might have a filetype of hex F74.

In this example, from robuild-client itself, we set a single environment variable, and then give it 3 commands to run. Those are just CLI commands, so you could can do a lot more if you wanted - the `!BuildAll` is an Obey file, which builds the libraries and then uses them to build the tool. The final copy to the clipboard is how we communicate the output to the service.

There's fuller documentation of the file format on the JFPatch-as-a-service site.

Before I move on to talk about how the service works, let's see if there any on the front end service itself.

---

# 3. How The Service Works

---

# How The Service Works
## What is the service made of? (1)

* Infrastructure - The infrastructure runs on AWS - Amazon Web Services - on their virtual machines, with a load blancer and a small server which is running constantly. It could scale, but it's set up to not do so - the infrastructure costs about $24/month, which is about $20 too much, but I've not got around to streamlining it. It's a 'meh' at the moment.

* FrontEnd - Static site is built with HSC, entirely custom colouring mode for ARM, BASIC and JFPatch, which are open source. The new colouring modes were open sourced a while back, and you'll find them on GitHub.

* BackEnd - Two Python flask applications - 1 for websockets, 1 for JSON which accept the requests for builds and dispatch them to RISC OS, providing the conduit for the output and throwback.

* The back end service uses a python module that I open sourced a while back which handles RISC OS extension data in Zip files.

* Tools - JFPatch itself. JFPatch was itself one of the easiest things to change for this environment. It needed a way to pass the built binary out to the caller - previously it just wrote to whatever file you supplied. Within the service it uses a new option where JFPatch copies the binary to the clipboard using the ClipboardHolder module. The module has a slightly modified implementation - instead of just holding the file, it makes a HTTP request to deliver the content to the back end. The back end code can then return the built binary to the user.

* JFPatch itself is written in BASIC which is tedious to work with on non-RISC OS systems, so all those files are actually all stored in text form and tokenised with a new tool I created using the code I had used for HdrToH.

---

# How The Service Works
## What is the service made of? (2)

This is the basic structure of the system

- User accesses the site.
- AWS directs the request to the back end server.
- Back end server decides what we need to do.
- Server calls in to the RISC OS system.
- RISC OS runs the build.
- The result comes back to the server through HTTP.

It's probably worth noting at this point that no part of the service uses the cross-compiling tools - it's RISC OS all the way when it comes to running what you submit.

That diagram is up on the site, so let's look at what goes on below that.

---
# How The Service Works
## What runs those services? (1)

There's actually 3 different ways that the JFPatch-as-a-service system can be invoked - as the JSON server, as the WebSockets server, and as a CLI tool.

The CLI tool is only ever used for debugging, so never gets used by the service itself, but it exists to make it possible to test the build process without having the JSON API or WebSockets API get in the way.

<step>

When the CLI tool is used, it has to manually do parse out the parameters that would have been delivered to the WebSockets interface through their parameters. The CLI system can use the simple JSON interface, or the streaming interface like the WebSockets system.

<step>

There's a 'build' object that knows how to run things. And there's a streaming version of that object - the difference is that the streaming version communicates the progress and results through a 'results' object that can talk to the caller. That's how the WebSockets system, and the CLI system in stream mode, get back information.

<step>

And once the build process is completed, we report back to the CLI or server with an appropriate final result.

Let's go look inside what happens in the build then.

---
# How The Service Works
## What runs those services? (5)

As we saw, we create a result object to hold information - even for the non-streaming case, we have one of these. It just holds the information, rather than routing it to anywhere.

<step>

The `rosource` module decides what filetype we're trying to handle. It uses some truly terrible heuristics to do this. Like 'is there an all caps PRINT in the file', or 'is there a #include in the file'. That only applies to single files, though - Zips are easy to recognise and extract.

We use the RISC OS Zip library I mentioned earlier to extract the files from the file. If they've got RISC OS types we use them. If they haven't got RISC OS types then we try to infer them where we can. All the files are stored in standard format for RISC OS on Unix - using the `,xxx` extensions.

Whether it's a single file, or many from a Zip, the result gets put in a temporary directory.

<step>

The `robuild` module looks at the files that were extracted and decides what type of build it is, and constructs a RISC OS command line that can be used to perform the build. If we extracted a `robuild.yaml` file, we parse the YAML file to give the commands and parameters for the build. I avoid some of the problems with YAML files being overly complex by not supporting them - the `simpleyaml` module is a package a wrote for just that purpose (that's open source too).

<step>

At this point we know what the build is going to look like, so we need to create a HTTP server to receive throwback and clipboard data from JFPatch (or whatever we end up running). Hopefully you remember that I'm using the clipboard to communicate the produced binary, but I've also got a version of DDEUtils that sends throwback data to a HTTP server as well. This way, we get structured information that can be fed back to the caller.

If those servers are called, they'll feed the data to the `results` object, which we created earlier on.

<step>

Finally we're ready to run the build. The `docker` module constructs docker command lines, which includes routing the HTTP ports properly, and setting up the commands to run RISC OS. The `streamedinput` module, which is open source, handles receiving lines from the Docker process and feeding them to the results object.

Finally, there's the `pyro` module that builds the actual command to run RISC OS.

<step>

And there we have the 3 different command line sequences getting built up to run the RISC OS command in a safe an isolated way on the Linux server, and get the results back to the user.

---
# How The Service Works
## Wait what?

It'd be nice if someone were to ask this question... but it's probably obvious from what I've been saying - and from the public documentation - that it's running RISC OS on a Linux system.

Before I move on to talk about how that works, let's see if there any on the back end of the JFPatch-as-a-service system.

---

# ...

We're jumping back in time now - into that gap that said we’d come back to.
I spent some time porting tools and making things build on macOS and Linux so that I could build a reasonable portion of the software that had gone into source control.

If you remember back in the 'Background' section, I talked about how important testing was. JFPatch-as-a-service is an amusing offshoot from the main goal of testing RISC OS things. It took about a month to create the service and site - just doing things in the evening after my day job (the build client and automation came later, but the main service isn't that different to how it was released).

But the service really just a bit of a distraction from the main work over the last year and a half.

---

# 4. RISC OS Pyromaniac

RISC OS Pyromaniac is the system that powers JFPatch-as-a-service. The name actually didn’t come until much later, but thanks to my friend Chris for it, otherwise it might still be called `testrun`.

So let's talk a little about how it came to be...


# RISC OS Pyromaniac
## How do you test RISC OS software without RISC OS?

My RiscPC is all the way on the other side of town.
It's not a good system anyhow - if something goes wrong, I'll have to reboot it and all those things I said before which make it bad for this sort of thing.

But...

<step>

The original versions of RISC OS - well, Arthur I guess - was semi-hosted from a BBC. BBC does all the keyboard and disc and video, but the ARM system does the actual running of programs.

That's what I want to be able to do - I want to be able to drive RISC OS from the CLI of a sane machine.

---
# RISC OS Pyromaniac
## Surely that's easy? (1)

Surely that's easy? You just run an emulation system until it hits a SWI and then you make the SWI do the I/O thing. Then you run some more?

<step>

Yup. That's the core of the Pyromaniac system.

So on June 10th last year I could run 'IfThere' from MacOS.

"Do the I/O thing" is actually a pretty involved process, so whilst I managed to get a tool running, what it ran wasn't actually that impressive. The 2 SWIs that worked were OS_File 17 to read whether the file was there, and OS_CLI which just reported an error saying it wasn't supported. But it ran.

---
# RISC OS Pyromaniac
## Surely that's easy? (2)

This is the basic execution sequence that was in place in the early system code for Pyromaniac.

- You load some code.
- You emulate it until something causes you to stop.
- Decide why it ended - Timers don't do much, Exceptions just terminate, and SWIs decide where to dispatch to.
- SWI return has to handle errors in the way that RISC OS expects
    - if the SWI was error reporting, we need to exit with an error
    - if the SWI has the X-bit set, we just set the V flag and return back to our emulation.
- And repeat until you're done.

The devil is in the detail, so it's never quite that easy.

But let's not dive heavily into how it works; let's start from what RISC OS Pyromaniac actually is...

---
# RISC OS Pyromaniac
## What is RISC OS Pyromaniac?


