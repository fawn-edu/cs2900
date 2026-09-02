#set document(
	title: [CS2900 Homework 1],
	author: "Fawn Sannar"
)

#show heading: set block(below: 1em)

#align(center)[
	#title()
	#context document.author.join("")
	#datetime.today().display()
]

#h(1.412em) Please be aware that this is not the first time I have done this particular assignment. I enrolled in this class during Fall 2025 (back when it was CS3100) but ended up withdrawing just after submitting this assignment. I did clean up the code and clarify my rationale (this time presented as comments in the source code), but if my rants about bit-shifting or S-block dimensionality seem familiar, you may have graded my submission from last year.

= Source

#raw(read("des_encrypt.c"), lang: "C", tab-size: 4)

= Output

#figure(
	image("output.png", width: 100%),
	caption: [Screenshot of DES encryption output],
)
