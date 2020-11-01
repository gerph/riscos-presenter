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

Good evening!

I don’t know how this talk will go over. But I hope that what you take away from it is something very different to what you were expecting when you joined. So, let’s go...

---

# Introduction
## How I'll do this talk

There are a lot of technology and features that I'll talk about. I'll call them out as I come to them. Some of what I’m talking about may not be familiar to RISC OS users, but may be much more common to users outside of RISC OS. If there's something you don't understand, and you feel you needs clarification, ask a question in the chat if you can. Someone else might be able to answer, or I may be able to address it immediately.

At the end of each section I'm going to have a short break to answer questions on the section. This should give us a chance to talk about that section’s topics without getting too far ahead and people being completely lost.

At the end of the talk, these slides and a bunch of links to resources will be made available.

---

# Introduction
## What we'll talk about

There are 5 sections to the talk -

- a little bit of background
- a discussion of what JFPatch-as-a-service is and what you can do with it
- a very light dive into how that service works,
- a look at the system that powers everything.
- and finally some conclusions.

At the end of the talk I’ll take questions on the subjects that have been covered here, for as long as people want to stick around.


---

# 1. Background

To look at the background we jump back about a year and a half, which will set the scene...

---

# Background
## Who am I?

You might know me as Justin Fletcher, or gerph. I changed my name a few years ago, and I use gerph where I want some continuity.

I'm a RISC OS architect, and in my day job, I'm a software engineer. I've worked with all of RISC OS from chip initialisation through to the applications and development tools. I'm pretty confident I know it better than most, even after 15 years away. Plus, as I will talk about, I’ve worked on many of the RISC OS interfaces more recently, which makes them more familiar.

If you want to know more about my past, the RISC OS Rambles on my website talk about it in much more detail.

---

# Background
## Dear gosh why?

So why did I start doing RISC OS things again?

Well, about a year and a half ago, I began to understand that I was very unhappy. I didn’t have anything that focused me. I didn’t work on RISC OS things because when I do, I get angry and upset. And then I’m useless for days. So I stayed away.

I’ve tried to deal with that, but things still set me off. Even little things can be a problem for me - I wrote the Rambles to try to be a cathartic release, but it wasn’t completely successful. So I stayed away from the RISC OS world.

I have regular counselling, and I believe that it’s been working - because I realised, about a year and a half ago, that there was something I could do.

I realised that I enjoy working with RISC OS, but I don’t enjoy the RISC OS community. I’m not intending to offend anyone here by that, but that’s what’s debilitating for me. Stopping myself from doing things with RISC OS, because I understand how badly it affects me, has been a way of coping. But I realised that I can do RISC OS things for myself, and not care about whether anyone else sees them. Just doing it because I enjoy it.

Well, wow. This was lightbulb moment for me.

That’s why I’m doing this talk - because I’m really proud of that realisation.
I can do things because I enjoy them, not because I feel I have to prove things to people.

That’s what this talk is. It’s my way of saying "this is what, most nights, makes laugh when I go to bed, chuckling ‘this is f’ing nuts’, ‘look how cute that is’. JFPatch-as-a-service is the bit I've made available to the world, but the rest ... we’ll get to that.


---

# Background
## So you want to use RISC OS but...

So I decided I would dust off some old source code and try to build some things. But there's some problems with that.

- Firstly, RISC OS development tools are a bit poor, and they only run on RISC OS. I don't have a RISC OS system.
Not only that, but if you've worked with DDT, the debugger, you'll know how much like russian roulette that is - if you see the insides of it you'll find out why.

- The next issue with RISC OS development is that projects on RISC OS have awful testing in my experience. That includes my own.

Ad-hoc testing is the way to that most things were done and the mantra of 'it's compiled? ship it!' seems to apply. Maybe that's not true for developers these days, but that was how it was. I wanted to be able to address that, if I was to do anything substantial.

- The final issue is that RISC OS itself is not a good system for running tests on. It's cooperative and single tasking. Generally in testing the 'System Under Test' does not cooperate - it will hang, crash, and leak memory like it's going out of style. And on RISC OS that commonly means its time to reboot.

Plus there's no isolation, so even if it doesn't look like the system is broken, it might have corrupted something important. I needed to do something about that.

---

# Background
# How does the real world do things?

Ok, so we've understood some problems; let's see how the real world does things.

These are just a few of the things that in the real world we expect to have, and which I'm going to look at.

There's:

- Source control
- Compiling code for RISC OS from other systems
- Controlling your development environment
- Automated testing in different ways
- And having fleets of machines to make things faster

Let's look at each of these and see how I addressed them.

---

# Background
## How can I do this? (1)

My own source had been in CVS for many years, and in about 2016 I moved everything - even my own ancient RISC OS projects - into git. I have a large number of python and perl projects managed in git as well. It's so much easier to work with than CVS. And even for that one little script you're just tinkering with, you can create a repository for it.

Git is a source control system that is intended for large scale distributed use, but which works even for a single user working with a single file.

It doesn't need a server, but if you want to share code that's one way you can do it. GitHub is one of the best known systems for storing and accessing things, which people will almost certainly have heard of. However, at the time GitHub wasn't so friendly to private projects, and even still, I didn't want my projects going off site. So I chose to use GitLab - it's installed on my server, and... it's pretty great. Oh, and it's free.

<step>

So GitLab is the tech that I use for my source control.

I was amused - but not at all surprised - that ROOL selected GitLab as their server system, a few years ago.

---

# Background
## How can I do this? (2)

So I've got a way of storing and managing the source, but I want to at least be able to compile and assemble things if I'm going to work on them.

It's all well and good being able to build RISC OS components for testing on macOS by stubbing libraries, but I want to be able to build things that will run on RISC OS.

<step>

Back in 2005 I had cross-compiling working for RISC OS, and most of the last version of RISC OS that I worked on were actually built on Linux.

However, that was all for 32bit Linux and Windows. So I ported the toolchain to work in 64bit compilers.

It wasn't actually 'simple', and I still see bugs now and then for cases I've missed. The authors loved their bitfields and packing structures into different types. But with a little work, the toolchain was made to compile RISC OS components on macOS.

All the cross-compiling tools have the prefix 'riscos hyphen' to make it easier to distinguish them. So you can see that 'riscos-amu', 'riscos-objasm', and 'riscos-link' were invoked in this example.

I needed to test the C compiler with some nice example code to see that it fared well. I had a lot of my own code, but using random stuff from other people to throw at the compiler highlights many problems you wouldn't otherwise find.

<step>

So I wrote a small library that downloaded all the C code from the Rosetta Code website. If you don't know Rosetta Code, it's a website containing solutions for many problems, in many different programming languages.

And having downloaded all the programs, I ran them through the C compiler to see whether it crashed or not.

---

# Background
## How can I do this? (3)

So the tools for building exist, and I have headers and libraries that I can use to link against.
But I need to get them to my machine.

Traditionally you downloaded and installed a package that gave you the build tools. And then you used them until the next time you needed to upgrade. In modern development, these tools and libraries change rapidly.

And even if they don't, you want more control over what exactly goes into building your latest product.

This is usually managed through a software repository that holds all those components that you might want - your toolchain, libraries, documentation, even full releases. These can be stored in the repository and retrieved in managed ways. In this way what you're using is reproducible, because everything that went into making it is known, and can be used again.

<step>

I chose Artifactory. It's not great. It's not terrible. It does the job. And it's free.
And I created some small scripts that let me push and pull my RISC OS components in a structured way.

---
# Background
## How can I do this? (4)

Sometimes I don't want to download the toolchain and set up binaries. I just want to compile some code. Normally you would just install the toolchain and your libraries on the system in that case. Then they'd be there when you needed them.

Ok, you can do that. But modern build systems have been taking advantage of 'containers'. If you've not come across containers or 'docker' before, then I'll try to summarise what it is in a couple of sentences.

Containers are a way of bundling all the resources you need - and only those that you need - to do a task into a single package and let you run just that package. Docker allows this to be done within Linux systems, by packaging just the bits of the OS and the tool that you need, and then running that in isolation.

If you're thinking that's kinda like a RISC OS application that included the modules it wanted, it's a bit like that, but much more - if the application contained the bits of the OS and the boot or system resources, you're getting closer.

<step>

And that's the technology used here - it bundles the RISC OS development tools into a docker container, then lets me build my source with those tools. You can see the docker command is a little unweildy. I actually have a small script called 'riscos-build' which contains the bulk of that command, so I don't run the full command except when demonstrating it.

In this example, I'm building one of my old modules using the new toolchain.

---

# Background
## How can I do this? (5)

The next feature of modern development on my list was automated testing. This is very closely associated with the process of 'Continuous Integration' and the terms are often used interchangeably. The distinction to draw here is between ad-hoc testing, where you test what you think is going to be a problem by hand, and automated testing, where you define tests which are run automatically.

- In ad-hoc testing, you essentially sign off that you think it's good enough.
- In automated testing, your tests tell you whether it's good enough.

Because the tests are automatically run, you find out quickly that things have broken, and you don't reintroduce earlier bugs - assuming you've written tests for them. Specifically that's feature and regression testing - there are other types of tests that can be run automatically, and you can write your tests to do whatever you want.

<step>

The GitLab source control system has a test system built in. This lets you run whatever tests you need to. When a change is pushed to the GitLab server, the tests are triggered.

In my case, these tests will:

- download the toolchain and environment from Artifactory
- build the code
- and then push the built result back up to Artifactory if it was successful.

If it wasn't successful, I'll get an email, and the website reports the build log.

---
# Background
## How can I do this? (6)

If I'm building, let's say, a RISC OS module, it'd be normal to write all the code in little components that you can try out separately, before bundling them together into the scary module enviroment. For a lot of code, you can test those components without needing the rest of the system. And for a lot of code, you can do that on non-RISC OS systems.

If you're just managing data structures, you don't need a real RISC OS system to check that the logic works right, for example. So in those cases you can make your code compile and test on a different platform. That's one way of doing things - and it's very effective.

In fact, just last month Jason Tribbeck was explaining exactly that process for his unit testing of the sound system.

<step>

But I really want to run things on RISC OS, because that environment that modules run in is a bit different, and you'll want to communicate with other parts of the system in situ. In particular, modules run in SVC mode - the mode with the highest priviledge and therefore the greatest power to make it a Bad Day for you.

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

Let's talk about JFPatch-as-a-service.

We spin forward a year - we'll cover the missing time later.

---

# JFPatch-as-a-Service
## Why?

In early March, David Thomas semi-challenged me to make some of the things I'd been doing public. I was still nervous about that, and wasn't really comfortable making that kind of announcement.

But the idea of doing something for April 1st, which wasn't actually an April fool, was appealing. I genuinely expected the response to be a 'meh', but that wasn't the point.

The whole point of the work I was doing was to be enjoyable - and I like solving problems, particularly when those problems are strange. It'd been floating around in my head for a while to do 'something' like it, but it hadn't really solidified until then.

Running an on demand RISC OS build process on a cloud system as a service is probably unique. I've been away and I don't pay attention to the comings and goings, so maybe it's completely passe, but I liked the idea.

And I like the idea of surprising people with something they didn't expect.

---
# JFPatch-as-a-Service
## What is JFPatch?

JFPatch... it's an assembler I wrote when I was at school... something like 25 years ago.
I learnt assembler by reading other people's code and patching it to do different things. Largely that was by writing a BASIC program that loaded the code, assembled some replacement code and saved it again.

That process is tedious. I wanted a shorthand for doing that boilerplate work. So I created a pre-processor that built those BASIC programs from a 'patch file'. I knew I shouldn't pollute the namespace, so the tool got a prefix of my initials.

It's actually not that clever, but it can construct a lot of the boilerplate that goes into writing modules. If you want a module that registers itself with a few vectors you can do that in just a couple of lines of code. It even goes so far to let you register a whole filesystem  through some simple definitions in the header.

---

# JFPatch-as-a-Service
## What is the service?

What's the service?

The service is a front end that lets you upload JFPatch code, builds it on a cloud server, and then returns the result to you.

I wanted it to be a tool that could be used like Matt Godbolt's compiler explorer. If you've not seen it, Compiler Explorer is a website that lets you enter some code and examine what the compiler makes of it. What instructions it compiles to and how they're ordered.

It does a different thing to JFPatch-as-a-service, and my work isn't anywhere near as polished and professional as that, but... that's where the idea and structure came from. Matt's an ex-RISC OS user, too.

---
# JFPatch-as-a-Service
## What can you build with the service? (1)

But what can the service do? Well, it can build any JFPatch code you throw at it, and now the module header is 32bit safe as well. I was too lazy to look at the 32bit header on the modules until recently.

But, the service can also build C, and Pascal and run BASIC programs, and build ObjAsm code.
If you give it a zip archive containing an AMU Makefile, it would invoke it and return the binary it thought was produced by it.

You can try this out on the real site if you'd like. Here's what you see...

---
# JFPatch-as-a-Service
## What can you build with the service? (2)

<use xc to turn on the cursor and point to bits>

- The editor lets you enter the code you want to build.
- Your code can then be sent to the server for building, and the build output will be shown to you.
- Any output files produced will be available for download.

This is a small C file that I wrote, and then sent to the service. You can see in the Build output at the bottom that it has built, and the result was returned as an Absolute file.

---
# JFPatch-as-a-Service
## What can you build with the service? (3)

A very simple Perl program. There's no returned binary from this, because we just printed a message.

You can see the 'Hello world' message in the build output.

---
# JFPatch-as-a-Service
## What can you build with the service? (4)

And finally a BBC BASIC program that also does very little.

As I mentioned, it did all this from April 1st. I had kinda hoped that someone would play with it and find that it did more things. I know I would have. It's not hidden, the source on the website says explicitly that it supports these things, and describes which languages are recognised.

Nobody came to talk to me about it, so that 'easter egg' remained hidden.

---
# JFPatch-as-a-Service
## What might use the service?

In the time since then, I’ve converted a few other RISC OS tools to be able to be automatically built. They’re on github with the label `riscos-ci`.

Some of the tools were very simple to make work - Rick Murray's ErrorCancel for example was just a simple matter of adding the right build commands to the repository. Others were a little more involved, like Julie Stamp's CObey module, which I had to tweak the code of, because I'm using an older compiler.

Running a build automatically on submitting the code is a great way of checking that it still works. Presumably you've built it yourself, but it never hurts to have an automated system check that.

It is also possible to do a 'release' of the code automatically in GitHub, so you can actually get a distributable archives out from the build.

If you check the DDEUtilsJF link, you'll see that there are 'releases' listed on the right of the page - those were automatically created after push. They just have to be approved by clicking a button in GitHub.

---
# JFPatch-as-a-Service
## How do you use the service?

How do you use it?

The service can be used 3 ways - either through the web interface, as you've seen, or by using the API. A service isn't really a service unless it can be used programatically.

There are two APIs for accessing the service - either a JSON HTTP API, or a WebSockets API.

The JSON API is more limited because you basically just post data to it and get back a response. It's also got a timeout of about a minute, which for some jobs may not be long enough.

The WebSockets API however, is an interactive system. If you're not familiar with WebSockets, then that's not especially surprising. They've been around since 2010, but unless you've been working on heavily interactive sites you probably wouldn't encounter them.

Unlike the regular HTTP request-response protocol, WebSockets allow bi-directional communication. That means you can send a message to the other end, and you can receive them. Packets are always delivered in order, and always delivered completely. Their content is application defined, so once established you can send anything you want.

JFPatch-as-a-service uses commands in JSON over WebSockets to request the server do builds for it.

---
# JFPatch-as-a-Service
## How do you use the JSON API?

How do you use the JSON API?

Just use a URL library. The URLFetchers on RISC OS would do just as well, but probably you wouldn't need to invoke a cloud-based RISC OS build client from a RISC OS system. But you could.

And of course, you can parse the JSON response with any JSON library you'd like.

This is the sort of request and response you would have for building with the API.

---
# JFPatch-as-a-Service
## How do you use the WebSockets API?

The WebSockets system is a little harder to work with, though. You definitely need a client to talk to it.

Fortunately, in the JFPatch examples repository I supplied python a client that uses a package which does the work for you. This is an example of using 'wsclient.py' to build the same code.

But that's not always a suitable solution - it means you have to install other Python packages,
and you have to have Python in the first place.

For those cases I created the `robuild-client`.

---

# JFPatch-as-a-Service
## What is the robuild-client?

The `robuild-client` tool is a little tool that's able to take a file, and give it to the service over the WebSockets interface. It can then report the output as it is produced by the build process. Finally, it can then save the output to a known location, or report an error through the exit status of the command in the normal way.

This makes it easy to work into a build pipeline.

The Github project builds a Linux version of the tool...

<step>

... and then using that tool it submits its own code to the service, so that we get a RISC OS build out.

Why would you want to have run it from RISC OS? I don't know, but completeness says if it's a tool for RISC OS, maybe it should run on RISC OS. More specifically, I can't completely repress that feeling that people in the RISC OS world reject anything that isn't for RISC OS as irrelevant, so I have to make it work for RISC OS too.

I used two libraries to do this - a JSON parser, and a WebSockets library. The JSON parser was originally written by David Gamble, a friend who used to do RISC OS things, although I didn't discover this until afterward.

The repository is also an example of producing releases from an automated build, as well as building RISC OS code from a non-RISC OS code base using `sed` to fix incompatible things.

I didn't say it was a *good* solution!

---

# JFPatch-as-a-Service
## How does the service know what to do?

How does it know what to do?

For simple files, it spots the language from its format. For example, if it's got BASIC tokens in then it's a BASIC file. If it has C comments in, it's a C file. The heuristic is pretty poor, but it works on pretty much anything but simple cases.

Zip archives can contain multiple files, and the services picks out ones that it can understand. If there's a Makefile in the file that it recognises, it'll invoke AMU on the file. If there's a JFPatch file, it'll run that file - which allows you to patch other files.

Hoping the service guesses right isn't ideal, so there's more control available.

If the Zip archive contains a `.robuild.yaml` file, this will be used to control how the build runs.

<step>

YAML stands for 'Yet Another Markup Language'. It's nicer to work with than JSON, so long as you avoid the many crazy cases it has. It's pretty nutty in extremis, but it's quite readable.

Although I say the file is called `.robuild.yaml` on RISC OS that would be `/robuild/yaml`, and it might have a filetype of hex F74.

In this example, from robuild-client itself, we set a single environment variable, and then give it 3 commands to run. Those are just CLI commands, so you could can do a lot more if you wanted - the `!BuildAll` is an Obey file, which builds the libraries and then uses them to build the tool. The final copy to the clipboard is how we communicate the output to the service.

There's fuller documentation of the file format on the JFPatch-as-a-service website.

Before I move on to talk about how the service works, let's see if there any question on the front end of the service.

---

# 3. How The Service Works

I've talked a little about what the service offers and how you drive it.
So now I'm going to talk about how it achieves that in the back end.

---

# How The Service Works
## What is the service made of? (1)

What is the service made of?

* Infrastructure - The infrastructure runs on AWS - Amazon Web Services - on their virtual machines, with a load blancer and a small server which is running constantly. It could scale, but it's set up to not do so - the infrastructure costs about $35/month, which is about $30 too much, but I've not got around to streamlining it. It's a 'meh' at the moment.

* FrontEnd - Static site is built with HSC, and uses a custom colouring mode for ARM, BASIC and JFPatch. The new colouring modes were open sourced a while back, and you'll find them on GitHub.

* BackEnd - Two Python flask applications provide the back end - 1 for websockets, 1 for JSON. These accept the requests for builds, and dispatch them to RISC OS, providing the conduit for the output and throwback.

* The back end service uses a python module which handles RISC OS extension data in Zip files. I open sourced that a while back.

* Tools - The tool is JFPatch itself. JFPatch was one of the easiest things to change for this environment. It needed a way to pass the built binary out to the caller - previously it just wrote to whatever file you supplied. Within the service it uses a new option to copy the output to the clipboard using the ClipboardHolder module. The back end code can then return the built binary to the user.

* JFPatch itself is written in BASIC which is tedious to work with on non-RISC OS systems, so all the source files are stored in text form and tokenised with a new tool I created.

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

That diagram is up on the JFPatch-as-a-service site, so let's look at what goes on below that.

---
# How The Service Works
## What runs those services? (1)

There's actually 3 different ways that the JFPatch-as-a-service system can be invoked - as the JSON server, as the WebSockets server, and as a CLI tool.

The CLI tool is only ever used for debugging, so never gets used by the service itself, but it exists to make it possible to test the build process without having the JSON API or WebSockets API get in the way.

<step>

When the CLI tool is used, it has to manually parse out the parameters that would have been delivered to the WebSockets interface. The CLI system can use the simple interface, or the streaming interface like the WebSockets system.

<step>

There's a 'build' object that knows how to run things. And there's a streaming version of that object - the difference is that the streaming version communicates the progress and results through a 'results' object that can talk to the caller. That's how the WebSockets system, and the CLI system in stream mode, get back information.

<step>

And once the build process is completed, we report back to the CLI or server with an appropriate final result.

Let's go look inside what happens in the build then.

---
# How The Service Works
## What runs those services? (5)

As we saw from the last slide, we create a result object to hold information - even for the non-streaming case, we have one of these. It just holds the information, rather than routing it to anywhere.

<step>

The `rosource` module decides what filetype we're trying to handle. It uses some truly terrible heuristics to do this. Like 'is there an all caps PRINT in the file', or 'is there a #include in the file'. That only applies to single files, though - Zips are easy to recognise and extract.

We use the RISC OS Zip library I mentioned earlier to extract the files from the archive. If they've got RISC OS types we use them. If they haven't got RISC OS types then we try to infer them where we can. All the files are stored in standard format for RISC OS on Unix - using the `,xxx` extensions.

Whether it's a single file, or many from a Zip, the result gets put in a temporary directory.

<step>

The `robuild` module looks at the files that were extracted and decides what type of build it is. It constructs a RISC OS command line that can be used to perform the build. If we extracted a `robuild.yaml` file, we parse the YAML file to give the commands and parameters for the build. I avoid some of the problems with YAML files being overly complex by not supporting them - the `simpleyaml` module is a package a wrote for just that purpose (that's open source too).

<step>

At this point we know what the build is going to look like, so we need to create a HTTP server to receive throwback and clipboard data from JFPatch (or whatever we end up running). The `pyroserver` module does this. Hopefully you remember that I'm using the clipboard to communicate the produced binary, but I've also got a version of DDEUtils that sends throwback data to a HTTP server as well. This way, we get structured information that can be fed back to the caller.

If those HTTP servers are called, they'll feed the data to the `results` object, which we created earlier on.

<step>

Finally we're ready to run the build. The `docker` module constructs docker command lines, which includes routing the HTTP ports properly, and setting up the commands to run RISC OS. The `streamedinput` module, which is open source, handles receiving lines from the Docker process and feeding them to the results object.

Finally, there's the `pyro` module that builds the actual command to run RISC OS.

<step>

And there we have the 3 different command line sequences getting built up to run the RISC OS command in a safe an isolated way on the Linux server, and get the results back to the user.

---
# How The Service Works
## Wait what?

It'd be nice if someone were to ask this question... but it's probably obvious from what I've been saying - and from the public documentation - that it's running RISC OS on a Linux system.

Before I move on to talk about how that works, let's see if there any on questions about this back end service.

---

# ...

We're jumping back in time now - into that gap that I said we’d come back to.

I 'dspent some time porting tools and making things build on macOS and Linux so that I could build a reasonable portion of the software that I had put into source control.

If you remember back in the 'Background' section, I talked about how important testing was. JFPatch-as-a-service is an amusing offshoot from the main goal of testing RISC OS things. It took about a month to create the service and site - just doing things in the evening after my day job.

The build client and automation came later, but the main service isn't that different to how it was originally released.

But the service was really just a bit of a distraction from the main work over the last year and a half.

---

# 4. RISC OS Pyromaniac

RISC OS Pyromaniac is the system that powers JFPatch-as-a-service. The name actually didn’t come until much later, but thanks to my friend Chris Johns for it, otherwise it might still be called `testrun`.

So let's talk a little about how it came to be...

---

# RISC OS Pyromaniac
## How do you test RISC OS software without RISC OS?

How do you test software without the operating system it runs on?

My RiscPC is all the way on the other side of town.
It's not a good system anyhow - if something goes wrong, I'll have to reboot it that's bad for testing.

But...

<step>

The early versions of RISC OS, when it was originally written, was semi-hosted from a BBC. The BBC does all the keyboard and disc and video, but the ARM system does the actual running of programs.

That's what I want to be able to do - I want to be able to drive RISC OS from the command line of a more sensible machine. In my case, the machine I use is my MacBook. Or my Linux server.

---
# RISC OS Pyromaniac
## Surely that's easy? (1)

Surely that's easy? You just run an emulation system until it hits a SWI and then you make the SWI do whatever I/O it needs. Then you emulate some more?

<step>

Yup. That's the core of the Pyromaniac system.

The first thing it ran, was a tool that calls a few system calls to check if a file exists and runs a command if so - the `IfThere` command.

The 'I/O' part of the process is actually a pretty involved, so whilst I managed to get a tool running, what it ran wasn't actually that impressive.

The 2 SWIs that worked were OS_File 17 to read whether the file was there, and OS_CLI which just reported an error saying it wasn't supported. But it ran.

Lots has been developed since then.

---
# RISC OS Pyromaniac
## Surely that's easy? (2)

This is the basic execution sequence in Pyromaniac.

<Turn the pointer on with 'xc' to indicate things>

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

It's an alternative implementation of RISC OS. It doesn't use what went before in the core system. It can still run the ARM binaries, through emulation, but the OS is written in a high level language.

And that's so that it can be used to try things out, and experiment. It's a lot easier in a high level language to replace an interface, or change the way that something works.

Similarly, it's a lot easier to reason about what the system is doing in a high level language. You're not bogged down in worrying about which flags are set, and what your registers hold.

And having been written from scratch, it's easier to make shortcuts for things you don't care about. There's a lot of the BBC OS heritage in there that you can just drop.

Did you know that the VDU system can change the orientation so that characters proceed down the screen rather than across the screen? Did you know that the system scrolls text sideways if you do that? Do you care? That's something that you can drop when it's not part of the environment that you are trying to exercise.

If you want to try it, that `VDU 23, 16`. And, actually, it's implemented in Pyromaniac as well. I just wanted an example of something obscure that most people would never have encountered.


---
# RISC OS Pyromaniac
## What's in a name?

Strictly 'Pyromaniac' is the system for running ARM code in a useful way - that’s the ‘hardware’ if you like. 'RISC OS' is what's implemented on top of that, the whole being 'RISC OS Pyromaniac',
but it doesn't really matter, and I refer to the whole as Pyromaniac.

You'll hear me refer to 'RISC OS Classic'; that's to distinguish the implementation of RISC OS. At one point I called it 'RISC OS Original', but that implies that it's been surpassed, which ain't the case!

‘RISC OS’ is the operating system interfaces. ‘RISC OS Classic’ is an implementation of those interfaces, and ‘RISC OS Pyromaniac’ is another implementation. Think of it like GNU/Hurd and GNU/Linux, if you know those systems.

---
# RISC OS Pyromaniac
## What makes up Pyromaniac? (1)

If you haven't guessed, it's written in Python.
It's largely modular internally, with subsystems separated into individual python files.

I use the Python Unicorn package to emulate the ARM code. I looked at a few different emulation solutions, but Unicorn is really nice.

Other than Unicorn, though, nothing else is required by the Pyromaniac system. If you need to run the UI, then you'll need GTK or Wx Python. If you want sound, you'll want the `rtmidi` system, and a MIDI synthesiser. But you don't /need/ them.

Without these modules, Pyromaniac will just track the current state. So if try to play a sound but the Python module isn't present, the call will say it was successful, but you won't get any sound. And if you're testing that the call works, that's good enough.

---
# RISC OS Pyromaniac
## What makes up Pyromaniac? (2)

This is the general architecture of RISC OS Pyromaniac at a high level.

<Turn the pointer on with 'xc'>

The bit you run is the harness, which just uses the Pyromaniac and RISC OS interfaces to start the system.

Pyromaniac itself provides the setup of the execution environment, and support functions. This is the only part of the system that communicates with Unicorn. So, were a different emulation system to be required, this could be substituted here.

The Kernel, and many python modules, sit above this and provide interfaces to RISC OS and its subsystems.
There's a separate python file for most of the subsystems.

By default Pyromaniac initialises only 1 RISC OS module - the UtilityModule. In this form, it's very limited, but that's what you want if you're providing a test and debug system. The other internal modules can be initialised by a single option, which brings in many of the common functions that you expect from the OS.

These Python RISC OS modules, which I've termed PyModules to distinguish them from the ARM implemented RISC OS modules, have almost all the functionality that their ARM counterparts have.

PyModules and ARM modules have exactly the same interface - internally they're both descendants of the same class, which means that to all intents and purposes they work the same way. However, instead of dispatching the RISC OS calls to Python as the PyModules do, they execute code through the ARM emulation.

And finally there's the application that you're running, which is always in ARM.

---
# RISC OS Pyromaniac
## How is it different from other systems?

So how is it different from other systems?

Probably most people know of RPCEmu and ArcEm. These are hardware emulators - they emulate as much of the machine as they can, and if they do their job right, the OS and applications are oblivious. Most of the work for that goes in to getting the hardware emulation right. There's no interface emulation at all - the OS that is running is the original OS with no changes.

Pyromaniac doesn't provide any hardware emulation at all. Pyromaniac works at the higher level of the OS interfaces.

Riscose may be a dim memory for some people. It was a project to provide an emulation at the SWI level for RISC OS applications and use an emulation system to handle the ARM code. That's quite similar to what Pyromaniac does.

There's a Linux Port of RISC OS which uses the host system's interfaces to trap execution on an ARM system, and emulation on non-ARM systems. Some of the interactions with the host system are through special interfaces, but if I've understood correctly, that's the exception rather than the norm. It's a hybrid system that emulates a hardware environment, with some interfaces to the host system.

Last month we were introduced to Amethyst, an ARM code execution system for unit testing on non-ARM systems. Amethyst isn't an OS emulator, but is intended to be used to check that individual units of code work properly.

Amethyst is incredibly similar to Pyromaniac in its goal of being able to test code on alien system, is the same, and it's using a very similar model. However, with Pyromaniac I wanted to be able to test the real interaction within the system.

The difference is one of intent really - Amethyst aims to unit test, and Pyromaniac aims to System test. They actually complement one another rather well.

If you haven't come across Wine, it's an x86 interface layer for Windows applications.

You take your Windows application and you run it on Linux or macOS with Wine. The Windows application thinks it is talking to Windows because that's the interface it calls, but those interfaces get translated to native operations on the native system. Because it's running the x86 code on an x86 system, it's not emulation, but it is replacing all the interfaces with those that have the same effect as the application was expecting.

That's kinda similar to what Pyromaniac does, although Pyromaniac is providing those interface replacements at the system call level, not the linker level.

Docker is the most different in terms of what it's doing - it uses the Linux kernel isolation and capabilities interfaces to isolate the execution of Linux applications. It's only superficially similar to Pyromaniac.

Rosetta is a technology used on macOS systems. It allows you to run the prior CPU architecture's applications on newer systems. The original version was to cope with the PowerPC to Intel transition. Rosetta 2 copes with the Intel to ARM transition.

In both cases, the application is translated from the old CPU architecture to the new one, but the interfaces that are invoked are the same - direct analogues of the interfaces exist in the new architecture. That allows the translation to be performed at the application level, and not to deal with the OS libraries that it uses.

Pyromaniac and Rosetta are similar in that they let you run applications from an alien architecture; otherwise they're quite different.

---
# RISC OS Pyromaniac
## How does it compare to a bare Operating System?

How does it compare to a bare Operating System?

Pyromaniac has many of the things you might expect in a regular operating system...

- It still has to worry about allocating memory, and taking calls from user space.
- It has to be able to handle I/O for those applications, whether it be keyboard, file system, sound or whatever.
- And it still has to have device drivers so that you can talk to the outside world.

But it doesn't have to worry about many of the things that a real OS does...

- You have no idea how glad I am to not have to worry about page tables, TLBs and cache coherency.
- Without real hardware, there's no need to provide interrupt masking, prioritisation, and bus discovery.
- And with no physical chips you don't have memory mapped devices to poke registers with. There's no clocks to set, no instruction set features to worry about.

I'm very happy to not deal with those.

---
# RISC OS Pyromaniac
## What does it mean?

What does that mean?

It's command line focussed so that you can run most command line tools. Remember, the goal of Pyromaniac is to be a testing and experimentation system, so the command line and core system interfaces are vital to that.

Because it's written in Python, and uses Unicorn which runs on different systems, Pyromaniac runs on macOS, Linux and Windows.

The core system is all new code written from scratch, although it doesn't cover all the OS. So when you run BASIC for example, you have to use the ARM version of BASIC. But the code that is the Operating System is all new.

The point of the system is to be able to test and find problems.
There's a lot of configurability for the system, and a large number of options for debugging and tracing code.

And...

<step>

Yes, I should acknowledge that this is technology that I've created. A pretty niche technology, but it makes me laugh. A lot.

---
# RISC OS Pyromaniac
## Command line only?

I asked a friend about my insecurities is doing this talk. I was worried about people saying "why are you telling us about this?", or not caring about an online service.

They replied "Nope, they'll be thinking 'Does it run Impression?'"

No. it does not run Impression.

There's no Desktop.
There's a lot of command line, although there is a graphics system.
And you can have graphics tools working without a desktop relatively well.

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

Normally on RISC OS you have a frame buffer - a region of memory that holds the bitmap of what you plotted. That doesn't exist on RISC OS Pyromaniac. In fact there may not even be a frame buffer on the host side either. Although normally you would have the Graphics implementation which used a bitmap on the host side, it doesn't have to.

Change a configuration option and instead of using a bitmap the graphics system creates an SVG - a vector graphic representation of the screen.

<step>

Finally there's the user interface - how you see the VDU and graphics. VDU output, as I've mentioned, can come out on the console, but the graphics output can be configured to be displayed in either wxWidgets or GTK - those are two different frameworks for applications. Or it can be configured to not be displayed at all.

<step>

And what you have to remember is that the VDU and graphics systems are pretty complicated. Remember that example for character direction from earlier? Well, there's a whole lot more where that came from.

---
# RISC OS Pyromaniac
## How do you use it? (1)

Ok, an example of how you actually use it.

This example runs the harness with the internal modules, and runs the command line.

The `*FX 0` command shows the OS version. There's no way to report the host system, because RISC OS wasn't really meant for that, so I tacked it on to the OS version.

And then, as you can see, I can run other commands, and exit the system - which takes us back to the calling shell.

---
# RISC OS Pyromaniac
## How do you use it? (2)

You can load modules at the command line, and run programs.

In this case, I'm creating a `Hello world` program in the file `myprog`, and running it.

The 'myprog' file has filetype `fd1`, which is represented in the standard format for non-RISC OS systems by using comma followed by the filetype. Type `fd1` is BASIC Text, which by default will run in BASIC.

On the command line I need to load BASIC before I can use it - it's not a part of RISC OS Pyromaniac. And you can see that it does print `Hello world`.

---
# RISC OS Pyromaniac
## Graphics demo!

Time for a demo of the graphics system.

That's what you have been watching for the past hour or so. This presentation has been executed entirely on RISC OS Pyromaniac.

<step>

So that's demonstrating...

- The Font system. This isn't the Classic FontManager; it's using fonts from the Cairo graphics system, which uses the host system's fonts. All the Font calls are implemented in the standard way using the Font SWIs. So it works just as it would on RISC OS Classic, albeit with different fonts.

- The vector system - In the corner, you'll see my 'G' logo. That's a DrawFile, being rendered through ImageFileRender. The DrawFile module just calls the Draw module to plot the content, and the Draw module has been implemented in RISC OS Pyromaniac to talk to Cairo's path primitives.

- The pictures you've seen to now are all PNGs. Like Drawfiles, those are rendered through ImageFileRender, with a custom ImageFileRender module which talks to Pillow, a Python image library.

- Hopefully you've seen no flicker as we moved between slides, because we use separate screen banks to render to the offscreen buffer before displaying it.

- You won't have seen the flicker, but you may have seen the mouse pointer appear when I've pointed to things, or in the form of the hourglass 'cog'. They're just standard RISC OS pointer definitions, albeit the hourglass is implemented entirely in Python.

- And there's key input - I press keys to move through the slides, or turn the pointer on and off.

The entire presentation tool was written just so that I could do this presentation.

---
# RISC OS Pyromaniac
## Graphics demo! (presentation)

A small digression before I talk more about Pyromaniac, there's a bunch of tech here that I've used or created.

The presentation tool I created for this talk will be open source.

It uses an open source markdown parser - all these slides are written in Markdown.

The Font selection is all through the FontMap module - which I created years ago to give an easy way to find variants of fonts. You ask it for a named font, but made oblique, and it'll do it.

The WebColours module was created for my SVG converter - give it a colour name and it gives you the colour. One job, does it well.

And I'm using ImageFileRender for the image rendering because... well, it does what it says in the tin.

---
# RISC OS Pyromaniac
## Features - What works?

Back to Pyromaniac and what it can do...

* System - We can run command line tools and modules. That meets most of the needs of my testing and CI goals.

* Interaction - You just use the keyboard as you might expect. In the UI, you can use the mouse, too.

* Video - We can draw things with the VDU interfaces; Draw and FontManager are provided. But there’s no frame buffer - direct screen access doesn’t exist. We also don’t have sprites.

* Sound - SoundChannels is provided, mapped to host MIDI devices. Each of the standard voice names is mapped to general MIDI instruments to approximate their sound. The sound system can actually work more like the BBC sound system if you’d prefer - I implemented the queueing system like the BBC because I had forgotten it didn’t work like that on RISC OS. So now you have a choice.

* Filesystem - You can access the local filesystem, so you can get the files you are testing into the system.
Or you can load FileSwitch and access regular RISC OS filesystems.

* Network - The host network stack can be accessed from RISC OS, and we try to map Socket calls through through - if the host has IPv6, `*IfConfig` will show you, and you can connect using those addresses.

* Compatibility - It's RISC OS, and whilst not all the interfaces have been implemented, many things do work. It depends on how complete those interfaces are. Sometimes you don’t care. Generally support for given interfaces varies between the level of Arthur and that of RISC OS Select.
The actual ‘features’ document is about 70K, and I'll make that available in the resources at the end.


---
# RISC OS Pyromaniac
## Features - What doesn't work?

What doesn't work?

* Desktop - although we don’t support the desktop, certain interfaces are provided to appease clients that expect them to exist. TaskWindow and TaskManager exist purely for this purpose.

* Sound - there’s no support for 8bit or 16bit sound. Although after Jason's talk last week, I was inspired to implement SoundDMA on an experimental branch. I can now play sounds with WaveSynth through Pyromaniac.

* Graphics - Sprites are harder because they’re direct access, and I’ve not got around to implementing them yet. Paletted modes are also hard, but they’re the more likely case for getting things wrong, so I implemented them first. I don’t have 24bit modes yet. That said, the paletted modes are actually implemented in the 24bit system, so you don’t get colour cycling with the current implementation either. Or flashing colours. The text cursor doesn’t flash, either. 

So many other bits are missing as well, as either less used cases, or things that I’ve just not got around to doing yet.


---
# RISC OS Pyromaniac
## Networking

The Internet module supports IPv4 and IPv6, which allows you to connect to, or serve to hosts on the local network or on the Internet. The interface enumeration, and socket operations are extended to take `sockaddr_in6` structures as you would expect. It’s actually not that different for most applications - `ifconfig` has been extended to be able to display IPv6 addresses.

The Resolver module can do lookups for names and addresses although currently it only supports IPv4 - it’s not been updated for IPv6 yet.

On the other hand, you can load the Classic MBufManager and Internet modules, and then the `EtherPyromaniac` module becomes useful - this provides a DCI4 driver which can be configured into a virtual network.

This virtual network is actually just a set of frames transmitted as JSON over a TCP connection. On the other end of the connection could be another Pyromaniac, or a 'virtual hub'. The virtual hub can connect many Pyromaniac instances together, and even connects to a tap which talks to a real network.

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

There's a few limitations, like it doesn't really support font control strings. And transforms aren't supported. But I'll probably get to them eventually. It does support encodings, though. If you request UTF-8 encoding for your font, it'll use that. This presentation uses UTF-8 for all its content.

The Classic FontManager works as well, but you have to turn off the bitmap caching by configuring the FontMax values to 0. If you do that, it just uses Draw directly for all the operations.

---
# RISC OS Pyromaniac
## Configuration

Ok, back to some core features...

Because Pyromaniac is intended as a debug tool, it needs to be configurable. It is not a case of one-size-fits-all.

Sometimes those configurations make the system ‘safer’ by removing access to the host systeem - the service uses these features to limit the host system access.

The first example here turns on the noisy ROM initialisation - this is akin to the option of the same name in the RISC OS Classic Kernel. It prints out more information about the ROM initialisation as the machine boots.

The second example changes the location in memory that hosts the ROM, and then shows where the modules were loaded.


---
# RISC OS Pyromaniac
## Configuration files

But you can also give it configuration files.
These make it easier to specify groups of options without long command lines.

This example is a part of the configuration file that I used for testing the booting of a RISC OS 5 ROM.

The format is YAML, which makes it relatively easy to read and write.

The configuration files make it a lot easier to use a group of settings reproducibly- which is especially important in testing.

---
# RISC OS Pyromaniac
## Tracing and debugging

The point of Pyromaniac is to make it easier to debug and test. It has a few different ways of reporting the progress of the code.

Tracing instructions is easy, although it slows down the execution considerably. Cheaper operations are to trace SWI calls, or to trap specific memory accesses. The instruction trace will also try to elide loops as well, so that thousands of repetitions of a tight loop don’t fill megabytes in the log file.

There's copious debug available, so you can turn on the debug for the subsystem you're seeing problems with.

---
# RISC OS Pyromaniac
## Tracing code (1)

It really depends on what you’re trying to find out as to what type of tracing or debugging you might want. The example here enables tracing of the SWI arguments when a SWI is called.
This debug option uses a built in table of names and types for the SWI parameters to show what those registers mean.

Here we see that the BASIC command to select the graphics colour has triggered a call to the ColourTrans module's SetGCOL system call. It shows what the parameters were on entry and exit.

The information on what those SWIs are was extracted from the OSLib def files - I wrote an OSLib `def` file parser which could extract the information into a form that we can use. The parser also generates templates for PyModules and other things. The parser is open source.

---
# RISC OS Pyromaniac
## Tracing code (2)

It’s also possible to trace the instruction execution explicitly.
In this mode the system is disassembling as it executes the ARM code. It can report on the registers and memory as it is running.

This means that it can include relevant information about what the registers and memory contains at the time of the execution.

This is one of the tests for Pyromaniac which exercises the reading of the current time as a string.

`SWI &FEED05` at the end is the return from the code being run to the operating system.

---
# RISC OS Pyromaniac
## Debugging

This example shows a couple of the subsystem debug options - the command line interpreter and the FSControl interfaces.

You can see the debug output interleaved with the program output. This shows what the system was doing at each stage.

The command line system is surprisingly complicated. I think I've got most of it right, but there's bound to be some differences.

---
# RISC OS Pyromaniac
## What is it like to work with? (1)

What is it like to work with?

I think the code is pretty well structured. Every function or method has access to the `ro` object, which is a reference to the root Pyromaniac object. This holds all the information for the system execution, including the Kernel object, which is where most internal resources can be accessed.

The Pyromaniac object also handles the configuration options, so that all components are configured through the same interfaces.

The whole system is able to be multiply instantiated - you can run different Operating System instances just by creating a new Pyromaniac object.

---
# RISC OS Pyromaniac
## What is it like to work with? (2)

The Kernel object holds all the subsystems. Some of those objects are simple - for example, the vectors object is really just an registration interface with a dispatcher.

Others, like the Dynamic Areas, are much more complex, as they allow many different operations to be performed on them through the RISC OS APIs.

RISC OS components implemented in Python can call any SWI through the System APIs object. This ensures that the calls get processed as if they had been issued by the classic system.

Let's have some quick examples...

---
# RISC OS Pyromaniac
## What is it like to work with? (3)

This is the implementation of the OS_ReadEscapeState SWI. It's a very simple system call which just returns whether the Escape key has been pressed in a flag.

As you can see from the implementation, that's what it does - it updates the C flag in the processor state to be reflect the escape condition from the program environment.

The registers object handles the numbered registers, but here the processor flags are being updated. In ARM these processor flags live in a virtual register.

Within registers object, there's a property setter which allows the flags to be manipulated as booleans without having to extract them from the flags word like a crazy ARM author would.

The function implementation is registered through a decorator on the function. This remembers the function as the handler for a given system call.


---
# RISC OS Pyromaniac
## What is it like to work with? (4)

This is a method within one of the PyModules, which implements the command to load a new RISC OS module.

Like the prior example it is very simple. It literally just calls the underlying system call.

Notice that there's no error handling present. There doesn't need to be - if an exception is raised by the code, it will be caught by the caller, and turned into a RISC OS error.

The function would only need to do more work if it had resources it needed to clean up.


---
# RISC OS Pyromaniac
## What is it like to work with? (5)

In ARM, you might allocate memory on the stack to store some temporary strings. That's not as easy in Pyromaniac - although it is possible - so it's simpler just to allocate some memory from a heap.

In this example, the `*Time` command needs somewhere to store the string it gets back before it can print it. A context handler allows memory to be allocated, and when the code block is exited, the memory will automatically be freed.

The result is written to the screen through the Kernel method `writeln`.
The Kernel object can actually be used as an i/o output, which means it is very easy to use like any other file handle.

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

I'd mentioned that errors are handled by the caller, but sometimes you need to trap them.
In ARM, you would call the SWI with the X-bit set and then check the error codes.

In Python, it's pretty much the same pattern, except that Python exceptions are used so checking the error code is pretty simple.

---
# RISC OS Pyromaniac
## What good is it? (1)

So it's an Operating System, but can you actually do anything with it? I wrote this presentation software with it, without touching any part of the Classic system. So in that regard it's been really successful.

I get a chance to try things out that I wouldn't otherwise be able to.

I mentioned Jason Tribbeck's Sound System earlie. During his talk last month, I knocked up a prototype using his documentation to see how well it fits together - I only spent about 3 hours on it, but the result is some hopefully useful feedback. The source to that 3 hours work is linked at the end of the talk.

And then there's the chance to debug other people's software.

Julie Stamp released an early version of an Obey module that they'd written. I was able to try it out and see how well it worked in a different environment.

Not just a plain 'this is what happened', but an actual recording in Asciinema of it.

---
# RISC OS Pyromaniac
## What good is it? (2)

This is the trace when it went wrong. Unfortunately they freed the stack that they were executing from. Then they returned, and had a bad day because they returned to invalid memory.

It's not a reversible debugger, so I have to deal with the aftermath.

But that's fine, because I run the command again with block tracing enabled - and because the system is deterministic I get exactly the same result. I can see that it hits the memset function to clear the stack, and then dies. Where the call comes from is easy to trace from its address.

---
# RISC OS Pyromaniac
## Problems...

Lots of stuff works, but there's stuff that doesn't, and stuff I still have to fix.

- As I mentioned previously, I don't have any interrupts, but timed events are still important. Parts of the network system will not work if they can't time out operations. The spinning hourglass you see now and then wouldn't work without timers. They're actually implemented with a check at the end of execution to see if anything is outstanding. It's tacky, but it's good enough.

- Whereas on Classic RISC OS the execution state would be entirely in the memory of the machine, in Pyromaniac it's split between Python and the emulated system. This is a problem that most emulation systems have, but it's worse because the OS state is also split between the two.

- My error handling is still not quite right. There's a design decision that I made early on that needs fixing. Because errors are handled as Python exceptions, I haven't differentiated between a signalling and a non-signalling error. It hasn't hurt yet, but it will.

- And whilst I'm not writing C or ARM code to poke hardware registers, I'm instead writing interfaces to Cairo, or SoundDevice, or GTK. It's replaced one problem area with another.

---
# RISC OS Pyromaniac
## Other technologies!

In doing this, I’ve created a few technologies, some of which I called out as I went along, but there are others:

- There’s a Python package for handling RISC OS alphabets as Python codecs.

- I created syntax colouring modes for the two editors I regularly use - SublimeText and Nano. And I’d mentioned earlier that I created the syntax mode for some RISC OS types in CodeMirror.

- I built some hourglass modules. Largely that was so that I understood how those hourglasses worked on RISC OS Classic before I implemented the one in Pyromaniac.

- To make it possible to test the compiler, I wrote a tool that allowed me to test that the tools work properly - it runs the tool, captures the output, compares to what’s expected. It’s an integration testing tool, but it works on RISC OS and on Linux/MacOS. For the toolchain it has support for parsing bits of the AOF/ALF/ELF files. For Pyromaniac testingit was used just in the text form. It’s Perl 5.0.0 compatible, intentionally so that you can run it with last Perl that I’ve got for RISC OS.

- I created a public repository of RISC OS tests, which uses this tool. The repository has a selection of the tests from Pyromaniac in it. However anyone could use them with RISC OS Classic, if they wanted, or people could contribute new test code that exercises other parts of the system.

- To document some of the things that I've been working on, I revisited the PRM-in-XML documentation, and updated my systems to built documets like that.

- And I created a changelog management system because I was fed up with dealing with conflicts when I tried to merge different branches.


---
# RISC OS Pyromaniac
## What does it run on?

I've done this presentation on macOS using the wxWidgets UI. But it runs just as well on Linux and Windows.

I've built applications that you can run on macOS and Windows, and I've built docker containers with the OS inside.

JFPatch-as-a-service is a customised docker container. It has JFPatch and other tools in it.
I have a private registry with my docker containers in, and the CI system updates and pushes to the registry so that they’re usable by other tests processes.

The Windows version was only made work properly in June, although it was largely just the console input that was a problem.
Automatic testing doesn’t happen on Windows yet.

---
# RISC OS Pyromaniac
## “Releases”?

Once a month I go through a release process - updating the features documentation, the main product documents, do manual tests on the different platforms, update the version number, and the like.

If I’m only doing it for myself, why go through that process? It’s another thing that helps me feel good about things - I can see what I’ve achieved.

There are some branches that have hung around for a while, because they're ok, but they don't make the cut for release. I have tasks on my board to review them, and raise issues when I find something is wrong.

There's a PyromaniacGit branch that's lets you use the host git client with RISC OS files; it's been there since June and still needs a lot of work.

<step>

I've not had a picture in quite a few slides, so these are the git branches I'm juggling. It shows the branches that I'm wrangling whilst I get Fonts and JPEG rendering to work together for this presentation.

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

But RISC OS Pyromaniac has tests for many of its interfaces. And runs them on multiple platforms. On each push.

Each of those 1000 tests starts RISC OS, executes the test and then exits it - so each environment is clean, and you’re only testing the things you’re interested in. Doing that with RISC OS Classic would be impractical - it's just not able to do that quickly and reliably.

I'm essentially booting RISC OS thousands of times per day to test its behaviour doesn't get worse. There aren't enough tests - there never are - but I try to make sure that at least some feature tests exists for each major change that happens.

And the code coverage for those tests is pretty decent too. Some things will always be hard to test, but it's looking good.


---
# Conclusion
## RISC OS is awful for testing

The other part of the statements about testing was that RISC OS is awful for testing.
After all, you may remember that the stated goal - the whole reason for starting doing this - was that I wanted to test things in a sensible way.

RISC OS Pyromaniac makes it possible to test in a less awful way. The environment can be controlled. Errors can be trapped. Execution time can be limited. And you can restart it.

JFPatch wasn't that bad to put through its tests. There are still some things to be improved, but it was important that I could see that I wasn't breaking things when I added the 32bit support. So I added tests first that it could parse the files, and then added the 32bit code. And then I could actually exercise some of the 32bit code on Pyromaniac.

---
# Conclusion
## Could it be better?

Could it be better? Well, of course!

There are huge swathes of things that I’ve not talked about here - subtleties, things that work, things that don’t, ideas that might never pan out but would be cool.

I could talk for an hour on any part of RISC OS Pyromaniac, and there’s so much more for me to do.

But I try not to feel too guilty about that, because the whole point of this was that I could do something I enjoy.

---
# Conclusion
## References

You'll find some reference material, including these slides, on pyromaniac.riscos.online.
Feel free to try out the shell server too.

But there's one more thing that I set out to do when I started all this work...

---
# Conclusion
## Am I happy?

The great and liberating thing about all this is that none of it matters one jot whether it happens.

The only thing that matters is whether I feel it would be fun and it would make me happy to do it.
That’s the thing that I learnt a year and a half ago, and that is what powers the service that you see - my happiness.

<pause>

---

# 6. Questions

That is the end of the planned talk… So I’ll take questions now.
