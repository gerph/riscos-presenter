background-colour: white
foreground-colour: black
body.font: Optima.Regular
body.size: 16pt
marginal.font: Optima.Regular
marginal.size: 16pt
padding: 2em
pre.padding: 0.5em
pre.border.type: solid
pre.border.width: 2px
pre.border.colour: slategray
pre.background-colour: gainsboro
item.padding.bottom: 0.5em
logo.bottom-right: GerphG
logo.bottom-right.inset: 1em
logo.bottom-right.width: 4em
logo.bottom-right.height: 4em
slidenumber.position: bottom-left
slidenumber.padding: 0.25em
slidenumber.format: $s / $m
h1.font: Optima.Bold
h1.size: 24pt
h1.padding.top: 0
h1.padding.bottom: 0
h2.font: Optima.Bold
h2.size: 20pt
h2.padding.bottom: 1.5em
vertical-align: top

# Building for RISC OS, Online
## and what makes it tick

---

# 0. Introduction.

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
## How can I do this? (3)

So the tools for building exist, and I have headers and libraries that I can use to link against.
But I need to get them to my machine.

Traditionally you downloaded and installed a package that gave you the build tools and you used them until the next time you needed to upgrade. In modern development, these tools and libraries change rapidly, and even if they don't you want more control over what exactly went in to building your latest product.

This is usually managed through a software repository that holds all those components that you might want - your toolchain, libraries, documentation, even full releases, can be stored in the repository and retrieved in managed ways. In this way what you're using is reproducible, because everything that went into making it is known, and can be used again.

<step>

I chose Artifactory. It's not great. It's not terrible. It does the job. And it's free.
And I created some small scripts that let me push and pull my RISC OS components in a structured way.

---
# Background
## How can I do this? (4)

Sometimes I don't want to download the toolchain and set up binaries. I just want to compile some code. Normally you would just install the toolchain and your libraries on the system in that case. Then they'd be there when you needed them.

Ok, you can do that. But modern build systems have been taking advantage of 'containers'. If you've not come across containers or one of the implementation systems - 'docker' - before, then I'll try to summarise in a couple of sentences.

Containers are a way of bundling all the resources you need - and only those that you need - to do a task into a single package and let you run just that package. Docker allows this to be done within Linux systems, by packaging just the bits of the OS and the tool that you need, and running that in isolation. If you're thinking that's kinda like a RISC OS application that included the modules it wanted, it's a bit like that, but much more - if the application contained the bits of the OS and the boot or system resources, you're getting closer.

<step>

And that's the technology used here - it bundles the RISC OS development tools into a docker container, then lets me build my source with those tools. You can see the docker command is a little unweildy. I actually have a small script called 'riscos-build' which contains the prefix, so I don't run the full command except when demonstrating it.

---

# Background
## How can I do this? (5)

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
## How can I do this? (6)

If I'm building, let's say, a RISC OS module, it'd be normal to write all the code in little components that you can try out separately, before bundling them together into the scary SVC enviroment. For a lot of code, you can test those components without needing the rest of the system. And for a lot of code, you can do that on non-RISC OS systems.

If you're just managing data structures, you don't need a real RISC OS system to check that the logic works right, for example. So in those cases you can make your code compile and test on a different platform. That's one way of doing things - and it's very effective.

<step>

But you really want to run things on RISC OS, because that scary SVC environment is a bit different, and you'll want to communicate with other parts of the system in situ.

We'll come to how you do that later.

---
# Background
## How can I do this? (7)

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
## What can you build with the service? (2)

That's a C file that I wrote, and then sent to the service, and you see that in the output from the process that it has built and the result was returned as an Absolute file.

---
# JFPatch-as-a-Service
## What can you build with the service? (3)

A very simple Perl program. There's no returned binary from this, because we just printed a message.

---
# JFPatch-as-a-Service
## What can you build with the service? (4)

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
## How do you use the service?

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

RISC OS Pyromaniac is the system that powers JFPatch-as-a-service. The name actually didn’t come until much later, but thanks to my friend Chris Johns for it, otherwise it might still be called `testrun`.

So let's talk a little about how it came to be...

---

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

It's an alternative implementation of RISC OS. It doesn't use what went before in the core system. It can still run the ARM binaries, through emulation, but the system at its core is written in a high level language.

And that's so that it can be used to try things out, and experiment. It's a lot easier in a high level language to replace an interface or change the way that something works, than in ARM.

Similarly, it's a lot easier to reason about what the system is doing in a high level language. You're not bogged down in worrying about which flags are set, and what your registers hold.

And having been written from scratch, it's easier to make shortcuts for things you don't care about.

Did you know that the VDU system can change the orientation so that characters proceed down the screen rather than across the screen? Did you know that the system scrolls text sideways if you do that? Do you care? That's something that you can drop when it's not part of the environment that you are trying to exercise.

That's `VDU 23, 16` if you want to see it on RISC OS Classic. And, actually, it's implemented in Pyromaniac as well. I just wanted an example of something obscure that most people would never have encountered.


---
# RISC OS Pyromaniac
## What's in a name?

Strictly 'Pyromaniac' is the system for running ARM code in a useful way - that’s the ‘hardware’ if you like. 'RISC OS' is what's implemented on top of that, the whole being 'RISC OS Pyromaniac',
but it doesn't really matter, and I refer to the whole as Pyromaniac. You'll hear me refer to 'RISC OS Classic'; that's to distinguish the implementation of RISC OS. At one point I called it 'RISC OS Original', but that implies that it's been surpassed, which ain't the case!

‘RISC OS’ is the operating system interfaces. ‘RISC OS Classic’ is an implementation of those interfaces, and ‘RISC OS Pyromaniac’ is another implementation. Think of it like GNU/Hurd and GNU/Linux.

---
# RISC OS Pyromaniac
## What makes up Pyromaniac? (1)

If you haven't guessed, it's written in Python. It's largely modular, although its structure aligns with many of the components of RISC OS Classic.

The Unicorn package provides an interface to the back end. I looked at a few different emulation solutions, but Unicorn is really nice. Plus it's written by the same person who wrote the Capstone packge that is used for disassembly.

Other than Unicorn, though, nothing else is required by the Pyromaniac system. For example, if you need to run the UI, then you'll need GTK or Wx Python. If you want sound, you'll want the `rtmidi` system, and a MIDI synthesiser.

Without these interfaces the principle that I follow in Pyromaniac is to work, by tracking state if the interfaces aren't present. It's a debugging and testing tool, so in some cases things just have to look right - they don't necessarily have to do anything.

---
# RISC OS Pyromaniac
## What makes up Pyromaniac? (2)

This is the general architecture of RISC OS Pyromaniac itself at a high level.

The bit you run is the harness, which just uses the Pyromaniac and RISC OS interfaces to start the system.

Pyromaniac itself provides the setup of the execution environment, the configuration system, tracing and a number of other support functions like threading. This is the only part of the system that communicates with Unicorn. So, were a different emulation system to be required, this could be substituted here.

The Kernel, and many python modules, sit above this and provide interfaces to RISC OS and its subsystems.
There's a separate python file for most of the subsystems, to make it easier to navigate, although some larger systems are split into multiple files.

By default RISC OS Pyromaniac initialises only 1 module - the UtilityModule. In this form, it's very limited, but that's what you want if you're providing a test and debug system. The internal pyromaniac modules can be initialised by a single option, which brings in many of the common functions that you expect from the OS.

These modules, which I've termed PyModules to distinguish them from the ARM implemented modules that we just call Modules, have almost all the functionality that their counterparts do. They can provide SWIs and commands; they can claim vectors and provide services; they can even be multiply instantiated.

The ARM modules have exactly the same interface - internally they're both represented by descendants of the same class, which means that to all intents and purposes they work the same way at the interface level. However, instead of dispatching the RISC OS calls to Python as the PyModules do, they execute code through the Pyromaniac ARM emulation.

And above that there's the application that you're running, which is always in ARM.

---
# RISC OS Pyromaniac
## How is it different from other systems?

So how is it different from other systems?

Probably most people know of RPCEmu and ArcEm. These are hardware emulators - they emulate as much of the machine as they can, and if they do their job right, RISC OS, as originally distributed is oblivious. Most of the work for that goes in to getting the hardware emulation right. There's no interface emulation at all - the OS that is running is the original OS with no changes.

Pyromaniac doesn't provide any hardware emulation at all. Pyromaniac works at the higher level of the OS interfaces.

Riscose may be a dim memory for some people. It was a project to provide an emulation at the SWI level for RISC OS applications and use an emulation system to handle the ARM code. That's incredibly similar to what Pyromaniac does.

There's a Linux Port which uses the host system's interfaces to trap execution on an ARM system, and emulation on non-ARM systems. Some of the interactions with the host system are through special interfaces, but if I've understood correctly, that's the exception rather than the norm. It's a hybrid system that emulates a hardware environment, with some interfaces to the host system.

If you haven't come across Wine, it's an x86 interface layer for Windows applications. You take your Windows application and you run it on Linux or macOS with Wine. The Windows application thinks it is talking to Windows because that's the interface it calls, but those interfaces get translated to native operations on Linux or macOS. Because it's running the x86 code on an x86 system, it's not emulation, but it is replacing all the interfaces with those that have the same effect as the application was expecting.

That's kinda similar to what Pyromaniac does, although Pyromaniac is providing those interface replacements at the system call level, not the linker level.

Docker is the most different in terms of what it's doing - it uses the Linux kernel isolation and capabilities interfaces to isolate the execution of Linux applications. However, from the user's perspective, you're running applications on a different system as if it's native, so superficially there are quite a few similarities.

Rosetta is a technology used on macOS systems to run the prior CPU architecture's applications on newer systems. The original version was to cope with the PowerPC to Intel transition. Rosetta 2 copes with the Intel to ARM tranisition. In both cases, the application is translated from the old CPU architecture to the new one, but the interfaces that are invoked are the same - direct analogues of the interfaces exist in the new architecture, which allows the translation to be performed at the application level, and not to deal with the OS libraries that it uses.

The main thing that is similar to Pyromaniac and Rosetta is that they intend to let you run programs from an alien architecture; otherwise they're quite different.

---
# RISC OS Pyromaniac
## What does it mean?

So it's largely command line version of RISC OS, which means you can run most command line tools. Remember, the goal of Pyromaniac is to be a testing and experimentation system, so the command line and core system interfaces are vital to that.

Because it's written in Python, and uses Unicorn which runs on different systems, RISC OS pyromaniac runs on macOS, Linux and Windows. Day to day, I run it on macOS, but the CI system on the server takes care of testing on Linux. Windows gets tested now and then through Wine and through Windows 10.

The core system is all new code written from scratch, although it doesn't cover all the OS, so when you run BASIC for example, you have to use the ARM version of BASIC. But the code that is Pyromaniac is all new.

The point of the system is to be able to test and find problems, so there's a lot of configurability for the system, and a large number of options for debugging and tracing code.

And...

<step>

Yes, I should acknowledge that this is technology that I've created. A pretty niche technology, but it makes me laugh. A lot.

---
# RISC OS Pyromaniac
## Command line only?

I asked a friend about my insecurities about doing this talk. I was worried about people saying "why are you telling us about this?", or not caring about an online service.

They replied "Nope, they'll be thinking 'Does it run Impression?'"

No. it does not run Impression.

There's no Desktop. There's a lot of command line, although there is a graphics system, and you can have graphics tools working without a desktop relatively well.

When you're running tests, you're isolating the functionality and the interactions, so command line is just fine.

---
# RISC OS Pyromaniac
## No graphics, then?

There is a distinction between parts of the system.

- There's the VDU system, which handles character output and VDU4 content.
- There's the Graphics system, which is involved with the OS_Plot calls, VDU5 output, Draw, Fonts and the like.
- There's the usually a frame buffer which is the bitmap of memory that holds that content.
- There's the user interface, which is how you see the VDU and graphics.

<step>

The VDU system is pretty well supported. It can output as a regular console application in the host OS for plain text. Or you can use the 'ANSIText' implementation, which translates the RISC OS VDU codes into ANSI terminal codes. This means that if you have yellow text in RISC OS, it comes out yellow in the terminal.

<step>

The graphics system is quite well supported too, and it looks pretty reasonable most of the time. There's still holes - for example GCOL actions other than plain plotting aren't supported yet. But by and large it works well enough for simple things to work.

<step>

Normally on RISC OS you have a frame buffer - a region of memory that holds the bitmap of what you plotted. That doesn't exist on RISC OS Pyromaniac. In fact there may not even be a frame buffer on the host side either. Although normally you would have the Graphics implementation be use a bitmap on the host side, it doesn't have to. Change a configuration option and instead of using a bitmap the graphics system creates an SVG - a vector graphic representation of the screen.

<step>

Finally there's the user interface - how you see the VDU and graphics. VDU output, as I've mentioned, can come out on the console, but the graphics output can be configured to be displayed in either wxWidgets or GTK - those are two different frameworks for applications. They functionality is similar. Or it can be configured to not be displayed at all.

<step>

And what you have to remember is that the VDU and graphics systems are pretty complicated. Remember that example for character direction from earlier? Well, there's a whole lot more where that came from.

---
# RISC OS Pyromaniac
## How do you use it? (1)

Ok, an example of how you actually use it.

This example runs the harness with the internal modules, and runs the supervisor.

The `*FX 0` shows the OS version, which describes itself together with the architecture. There's no way to report the host system, because RISC OS wasn't really meant for that, so I tacked it on to the OS version.

And then, as you can see, I can run other commands, and exit the system - which takes us back to the calling shell.

---
# RISC OS Pyromaniac
## How do you use it? (2)

You can load modules at the command line, and run programs.

In this case, I'm creating a `Hello world` program in the file myprog, and running it.

The 'myprog' file has filetype `fd1`, which is represented in the standard format for non-RISC OS systems by using comma followed by the filetype. Type `fd1` is BASIC Text, which by default will run in BASIC.

On the command line I need to load BASIC before I can use it - it's not a part of RISC OS Pyromaniac.

---
# RISC OS Pyromaniac
## Graphics demo!

Time for a demo of the graphics system.

That's what you have been watching for the past hour or so. This presentation has been executed entirely on RISC OS Pyromaniac.

<step>

So that's demonstrating...

- The Font system. This isn't the RISC OS FontManager; it's using fonts from the Cairo graphics system, which uses the host system's fonts. All the Font calls are implemented in the standard way, so it works just as it would on RISC OS Classic, albeit with different fonts.

- In the corner, you'll see my 'G' logo. That's a DrawFile, being rendered through ImageFileRender. The DrawFile module just calls the Draw module to plot the content, and the Draw module has been implemented in RISC OS Pyromaniac to talk to Cairo's path primitives.

- The pictures you've seen to now are all PNGs. Like Drawfiles, those are rendered through ImageFileRender, with a custom ImageFileRender module which talks to Pillow, a Python image library.

- Hopefully you've seen no flicker as we moved between slides, because we use separate screen banks to render to the offscreen buffer before displaying it. Whilst planning this presentation I got fed up with the ugly redraw as the presentation redrew the image live, so I separated the render context and the display context. Took about an hour. I'm very pleased with the result.

- You won't have seen the flicker, but you may have seen the mouse pointer appear when I've pointed to things, or in the form of the hourglass 'cog'. They're just standard RISC OS pointer definitions, albeit the hourglass is implemented entirely in Python.

- And there's key input - I press keys to move through the slides, or turn the pointer on and off.

---
# RISC OS Pyromaniac
## Graphics demo! (presentation)

Small digression before I talk more about Pyromaniac, there's a bunch of tech here that I've used or created.

The presentation tool I created just for this talk will be open source.

It uses an open source markdown parser - all these slides are written in Markdown.

The Font selection is all through the FontMap module - this was created years ago to give an easy way to find variants of fonts. You ask it for a named font, but made oblique, and it'll do it.

The WebColours module was created for my SVG converter - give it a colour name and it gives you the colour. One job, does it well.

And I'm using ImageFileRender for the image rendering because... well, it does what it says in the tin.

---
# RISC OS Pyromaniac
## Features - What works?

* System - We can run command line tools and modules. That meets most of the needs of my testing and CI goals.

* Interaction - You just use the keyboard as you might expect. That includes cursor keys - the terminal sequences are recognised and converted into the correct input to RISC OS. In the UI, you can use the mouse, too.

* Video - We can draw things with the VDU interfaces, Draw and FontManager are provided. But there’s no frame buffer - direct screen access doesn’t exist. We also don’t have sprites, either.

* Sound - SoundChannels is provided, mapped to host MIDI devices. Each of the standard voice names is mapped to general MIDI instruments to approximate their sound. The sound system can actually work more like the BBC sound system if you’d prefer - I implemented the queueing system like the BBC because I had forgotten it didn’t work like that on RISC OS. So now you have a choice.

* Filesystem - You can access the local filesystem - or you can load FileSwitch and access regular RISC OS filesystems.

* Network - The host network stack can be accessed from RISC OS, and we try to map Socket calls through through - if the host has IPv6, *IfConfig will show you, and you can connect using those addresses.

* Compatibility - It's RISC OS, but not all the interfaces have been implemented, so many things work. But it depends on how complete those interfaces are. Sometimes you don’t care. Generally support for given interfaces varies between the level of Arthur and that of RISC OS Select.
The actual ‘features’ document is about 70K, and I'll make that available in the resources at the end.


---
# RISC OS Pyromaniac
## Features - What doesn't work?


* Desktop - although we don’t support the desktop, certain interfaces are provided to appease clients that expect them to exist. TaskWindow and TaskManager exist purely for this purpose.

* Sound - there’s no support for 8bit or 16bit sound.

* Graphics - Sprites are harder because they’re direct access, and I’ve not got around to implementing them yet. Paletted modes are harder, but they’re the more likely case for getting things wrong, so I implemented them first, so we don’t have 24bit modes yet. That said, the paletted modes are actually implemented in the 24bit system, so you don’t get colour cycling with the current implementation either. Or flashing colours. The text cursor doesn’t flash, either. Plumbing is largely there, but... meh.

So many other bits are missing as well, as either less used cases, or things that I’ve just not got around to doing yet.


---
# RISC OS Pyromaniac
## Networking

The Internet module supports IPv4 and IPv6, which allows you to connect to, or serve to hosts on the local network or on the Internet. The interface enumeration, and socket operations are extended to take `sockaddr_in6` structures as you would expect. It’s actually not that different for most applications - ifconfig has been extended to be able to display IPv6 addresses.

The Resolver module can do lookups for names and addresses although currently it only supports IPv4 - it’s not been updated for IPv6 yet.

If, on the other hand, you load the Classic MBufManager and Internet modules, the 'EtherPyromaniac' module becomes useful - this provides a DCI4 driver which can be configured into a virtual network.

This virtual network is actually just a set of frames transmitted as JSON over a TCP connection. On the other end of the connection could be another Pyromaniac, or a ‘virtual switch’ which connects many Pyromaniac instances, or connects to a tap which talks to a real interface.

EasySockets is also supported, although it actually bypasses the Internet module entirely, and just uses the host network system. As a result, I connected to IRC servers quite early on in the development using the test programs, and without realising it, I had connected through IPv6. That would almost certainly have been the first use of RISC OS with IPv6.


---
# RISC OS Pyromaniac
## Draw module (1)

I've mentioned that I can render DrawFiles, and that it goes through the Cairo path rendering system. But it's also possible to use the Classic Draw module with DrawFile. In case you don't know, the Draw module puts lines on the screen by calling the `hline` interface - you can find that through `OS_ReadVduVariables`. On Select, it also uses the `polyhline` interface to work faster on accelerated systems.

In Pyromaniac though, `hline` is implemented as a call to the Cairo line drawing, so the Classic Draw module can still render to the screen. This demo doesn't use the Classic Draw module.

Obviously it's not just the simple 'G' that can be rendered...

---
# RISC OS Pyromaniac
## Draw module (2)

... my brother drew this about 25 years ago. It still amuses me.

---
# RISC OS Pyromaniac
## FontManager

I've mentioned the FontManager, but specifically it only uses the 'toy' interface that Cairo provides. At some point I'll look at using Pango to make it a lot more flexible, but it works pretty well for the simple things.

There's a few limitations, like it doesn't really support font control strings, and transforms aren't supported. But I'll probably get to them eventually. It does support encodings, though. If you request UTF-8 encoding for your font, it'll use that. In fact the presentation uses UTF-8.

The Classic FontManager works as well, but you have to turn off the bitmap caching by configuring the the FontMax values to 0. If you do that, it just uses Draw directly for all the operations.

---
# RISC OS Pyromaniac
## Configuration

Ok, back to some core features...

Because Pyromaniac is intended as a debug, testing and prototyping tool, it needs to be configurable. It is not a case of one-size-fits-all. Sometimes you want to see what happens when the system reacts differently. Sometimes you want to enable (or disable) a feature.

Sometimes those configurations make the system ‘safer’ by removing access to the host systeem - the service uses these features to cut back on the host system access.

The first example turns on the noisy ROM initialisation - this is akin to the option of the same name in the RISC OS Classic Kernel - and prints out more information about the ROM initialisation as the machine boots.

The second module changes the location the ROM lives at in memory, and runs the *Modules command.

I can demo those if I have time.


---
# RISC OS Pyromaniac
## Configuration files

But you can also give it configuration files, which collect together the options so that you have to manually give long command lines.

This is a part of the configuration file that I used for testing the booting of a RISC OS 5 ROM. In the interests of brevity I’ve elided almost all the unplugged modules, otherwise this wouldn’t fit on a slide.

The format is YAML, which makes it relatively easy to read and write. This makes it a lot easier to run things, and means that you don’t lose the configuration that made a particular test useful.

---
# RISC OS Pyromaniac
## Tracing and debugging

The whole point of Pyromaniac is to be able to make debugging and testing easier. It has a few different ways of reporting the progress of the code.

Tracing instructions is easy, although it slows down the execution considerably. Cheaper operations are to trace SWI calls, or to trap specific memory accesses. The instruction trace will also try to elide loops as well, so that thousands of repetitions of a tight loop don’t fill megabytes in the log file.

There's copious debug available, so usually you can turn on the debug for the subsystem you're seeing problems with.

---
# RISC OS Pyromaniac
## Tracing code (1)

It really depends on what you’re trying to find out as to what type of tracing or debugging you might want. The example here enables tracing of the SWI arguments when a SWI is called. The register dump can also be triggered, but this debug option uses a built in table of names and types for the SWI parameters to show what those registers mean.

The information on what those SWIs are was extracted from the OSLib def files - I wrote an OSLib `def` file parser which could extract the information into a form that we can use. The parser also generates templates for PyModules and other things.

---
# RISC OS Pyromaniac
## Tracing code (2)

It’s obviously possible to trace the instruction execution explicitly. In this mode the system is disassembling as it executes code, so it has a view on the registers and memory at the time that the code is executing. This means that it can include relevant information about what the registers and memory contains at the time of the execution.

`SWI &FEED05` at the end is the return from the Utility to the operating system.

---
# RISC OS Pyromaniac
## Debugging

This example shows a couple of the subsystem debug options - the command line interpreter and the FSControl interfaces. You can see the debug output interleaved with the program output, showing what was happening.

---
# RISC OS Pyromaniac
## FIXME - What's it useful for?

FIXME

---
# RISC OS Pyromaniac
## What is it like to work with? (1)

I think the code is pretty well structured. Pretty much every function or method has access to the `ro` object, which is a reference to the Pyromaniac object. This holds all the information for the system execution, including the Kernel object, which is where most internal resources can be accessed.

The Pyromaniac object also handles the configuration options, so that all components are configured through the same interfaces.

---
# RISC OS Pyromaniac
## What is it like to work with? (2)

The Kernel object holds all the subsystem objects. Some of those objects are simple - the vectors object is really just an registration and deregistration interface with a dispatcher, for example. Others, like the Dynamic Areas objects are much more complex, as they allow many different operations to be performed on them through the RISC OS APIs.

RISC OS components implemented in Python can call any SWI, or special call through the System APIs object, which ensures that the calls get processed as if they had been issued by the classic system.

Let's have some quick examples...

---
# RISC OS Pyromaniac
## What is it like to work with? (3)

This is the implementation of the OS_ReadEscapeState SWI. It's a very simple system call which just returns whether the Escape key has been pressed in a flag.

As you can see from the implementation, that's what it does - it updates the registers with the C flag in the CPSR to be the same as the state of the current escape condition in the program environment.

The registers object handles the numbered registers, but here the processor flags are being used. There's a property setter which allows the flags to be manipulated as booleans without having to extract them from the flags word.

The SWI is registered with the system through the decorator on the function. Decorators are a feature of Python which is generally used to modify the behaviour of functions. In this case, though, the function remains the same, but it is remembered as the handler for a given SWI.


---
# RISC OS Pyromaniac
## What is it like to work with? (4)

This is a method within a the OSCommands PyModule, which implements the RMLoad command. Like the prior example it is very simple. In RISC OS Classic that was intentional - many commands were just thin wrapper around the system call.

Notice that there's no error handling present. There doesn't need to be - if an exception is raised by the code, it will be caught by the caller. The function would only need to do more work if it had resources it needed to clean up.


---
# RISC OS Pyromaniac
## What is it like to work with? (5)

In ARM, you might allocate memory on the stack to store some temporary strings. That's not as easy in Pyromaniac - although it is possible - so it's simpler just to allocate some memory.

In this example, the `*Time` command needs somewhere to store the string it gets back before it can print it. A context handler allows a memory to be allocated, and when the code block is left, the memory will automatically be freed.

Writing to the VDU stream is just Kernel call to `writeln`. There Kernel object can actually be used as an io output, which means that you can pass it to any function that needs to write to the VDU output.

---
# RISC OS Pyromaniac
## What is it like to work with? (6)

This is the implementation of the *Show command. We use a context handler to ensure that whilst it’s running we’re in paged mode. That means that when the screen gets filled, you have to do something to let the output continue.

We enumerate through the variables that match the argument. Regular variables are printed with GSTrans escaping - the GSTrans module supports both decoding and encoding - and then we construct a suffix for the type of variable that’s present. Finally, we send the line all to the VDU system for printing.

Not everything is simple - some things that are made easy in RISC OS don’t have easy translations into the Python code - but mostly it’s just a matter of implementing what you need, as you need it. Fortunately, I know RISC OS pretty well.

A couple of months ago, I found myself saying “Dear gosh, MessageTrans_EnumerateTokens is tedious” as I found I needed to implement it. And then 10 minutes later, I had an implementation working. It’s a nice to find that something that’s a bit tedious on RISC OS actually only takes 10 minutes to implement. That’s not quite how I remember it being on RISC OS Classic.

---
# RISC OS Pyromaniac
## What is it like to work with? (7)

I'd mentioned that errors are handled by the caller, but sometimes you need to trap them. In ARM on RISC OS, you'd call the SWI with the X-bit set and then check the error codes.

In Python, it's pretty much the same pattern, except that Python exceptions are used.

---
# RISC OS Pyromaniac
## Problems...

FIXME

---
# RISC OS Pyromaniac
## Other technologies!

In doing this, I’ve created a few technologies:

- There’s a Python package for handling RISC OS alphabets as Python codecs.

- I created syntax colouring modes for the two editors I regularly use - SublimeText and Nano. And I’d mentioned earlier that I created the syntax mode for some RISC OS types in CodeMirror.

- I built some hourglass modules. Largely that was so that I understood how those hourglasses worked on RISC OS Classic before I implemented the one in Pyromaniac.

- To make it possible to test the compiler, I wrote a tool that allowed me to test that the tool is working properly - runs the tool, captures the output, compares to what’s expected. It’s an integration testing tool, but it works on RISC OS and on Linux/MacOS. For the toolchain it had support for parsing bits of the AOF/ALF/ELF files, but when I started with Pyromaniac it was used just in the text form. It’s Perl 5.0.0 compatible, intentionally so that you can run it with last Perl that I’ve got for RISC OS.

- The tool is bundled into a repository which holds a number of the test commands that I use with Pyromaniac; anyone could use them, if they wanted. Maybe people could test RISC OS Classic, or people could contribute new test code that exercises other parts of the system.

- To document some of the things that I've been working on, I revisited the PRM-in-XML documentation and updated my systems to built documets like that.

- And I created a changelog management system because I was fed up with dealing with conflicts when I tried to merge different branches of development.


---
# RISC OS Pyromaniac
## What does it run on?

This presentation has been performed using the wxWidgets UI on macOS.

JFPatch as a service is a customised docker container (it has JFPatch and other tools in it).
I have a private registry with my docker containers in, and the CI updates and pushes to the registry so that they’re usable by other CI processes.

The Windows version was only made work properly in June, although it was largely just the console input that was a problem. Testing doesn’t happen on Windows yet.

---
# RISC OS Pyromaniac
## “Releases”?

Once a month I go through a release process - updating the features documentation, the main product documents, and some tests on the different platforms, update the version number, etc.

If I’m only doing it for myself, why go through that process? It’s another thing that helps me feel good about things - I can see what I’ve achieved. Some months there’s very little - like in March this year the changes were pretty small, because I was creating the service. But other times there’s lots going on.

There are some branches that have hung around for a while, because they're ok, but they don't have enough features for me to feel comfortable with them being in the master branch.

There's a PyromaniacGit branch that's lets you use the host git client with RISC OS files; it's been there since June and still needs a lot of work.

<step>

I've not had a picture in a few slides, and this is a pretty ugly git tree whilst I'm trying to get the independant Font and JPEG branches into a state where they'll work together for the presentation.

---

# 5. Conclusion

Ok this concludes the section on Pyromaniac, but I'm not going to take questions just yet.

There's just one short section to finish up

---
# Conclusion
## Have I done what I set out to do?

If you remember back in the Background section, I said there were a few things that I set out to deal with. Let's look at each.

---
# Conclusion
## Development on RISC OS is tedious

These are the things that I saw as features of modern development.
Did I manage to achieve those goals?

<step>

(source control) - Yup, everything's in source control and I can access it.

<step>

(cross compiling) - Yup, I can build RISC OS components on macOS or Linux.

<step>

(managed development environments) - yup, I've got the multiple ways of controlling my builds.

<step>

(automated testing) - yup, externally I created the JFPatch-as-service site, and internally I use CI on most things.

<step>

(feature and regression testing) - yup, I have thousands of tests.

<step>

(fleets of systems available) - no, but mostly because I don't need to; I have a hefty server that can run multiple builds at once, and I've not had a need for more.

---
# Conclusion
## RISC OS Testing is awful

You remember I said that RISC OS testing is awful? Well it is for RISC OS Classic.

But RISC OS Pyromaniac has tests for many of its interfaces, and runs them on multiple platforms on each push. Each of those 1000 tests starts RISC OS, executes the test and then exits it - so each environment is clean, and you’re only testing the things you’re interested in. Doing that with RISC OS Classic would be ... interesting.

I'm essentially booting RISC OS thousands of times per day to test its behaviour doesn't get worse. There aren't enough tests - there never are - but I try to make sure that at least some feature tests exists for each major change that happens.

Coverage: There’s a lot of debug code that will never be hit, and some code like the UI is very difficult to exercise.


---
# Conclusion
## RISC OS is awful for testing

The other half of the statements about testing was that RISC OS is awful for testing.
After all, you may remember that the stated goal - the whole reason for starting doing this - was that I wanted to test things in a sensible way.

RISC OS Pyromaniac makes it possible to do this in a less awful way. The environment can be controlled. Errors can be trapped. Execution time can be limited. And you can restart it.

JFPatch wasn't that bad to put through its tests. There are still some things to be improved, but it was important that I could see that I wasn't breaking things when I added the 32bit support. So I added tests first that it could parse the files, and then added the 32bit code. And then I could actually exercise some of the 32bit code on Pyromaniac.

---
# Conclusion
## Could it be better?

Could it be better? Of course!

There are huge swathes of things that I’ve not talked about here - subtleties, things that work, things that don’t, ideas that might never pan out but would be cool.

I could talk for an hour on any part of RISC OS Pyromaniac, and there’s so much more for me to do.

But I try not to feel too guilty about that, because the whole point of this was to that I could do something I enjoy.

---
# Conclusion
## References

You'll find some reference material, including these slides, on SITE GOES HERE.

FIXME: Make the site.

But there's one more thing that I set out to do when I started all this work...

---
# Conclusion
## Am I happy?

The great and liberating thing about that is that none of it matters one jot whether it happens or not.

The only thing that matters is whether I feel it would be fun and it would make me happy to do it. That’s the thing that I learnt a year and a half ago, and that is what powers the service that you see - my happiness.

<pause>

---

# 6. Questions

That is the end of the planned talk… So I’ll take questions now.
