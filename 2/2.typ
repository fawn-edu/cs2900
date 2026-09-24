#set document(
	title: [CS2900 Homework 2],
	author: "Fawn Sannar"
)

#show heading: set block(below: 1em)
#show link: set text(fill: blue)

#align(center)[
	#title()
	#context document.author.join("")
	#datetime.today().display()
]

= Question 1

#quote(block: true, quotes: true)[
	Please develop a program that implements all the steps of the DES algorithm (Decryption only). You can choose any programming language. You will be doing the reverse of assignment 1.  Your input variables will need to allow for input of the value, so different use cases can be explored.  For a better understanding, please follow the steps mentioned in the below link.

	#link("https://page.math.tu-berlin.de/~kant/teaching/hess/krypto-ws2006/des.htm")[The DES Algorithm Illustrated]
]

== Source

#raw(read("des_decrypt.c"), lang: "C", tab-size: 4)

== Output

#figure(
	image("des_output.png", width: 100%),
	caption: [Screenshot of DES decryption output],
)

#pagebreak()

= Question 2

#quote(block: true, quotes: true)[
	Encrypt and decrypt using the RSA algorithm with the following system parameters: $p=3, q=11, e=7, x=5$
]

$
p &= 3 & "By question instructions" \
q &= 11 & "By question instructions" \
e &= 7 & "By question instructions" \
n &= p q & "By definition" \
&= 33 & "By arithmetic" \
phi (n) &= (p-1)(q-1) & "By definition" \
&= (3-1)(11-1) & "By substitution of" p, q \
&= 20 & "By arithmetic" \
1 &= (e d) mod phi (n) & "By definition" \
&= (7 d) mod 20 & "By substitution of" e, phi (n) \
d &= 3 & "By modular multiplicative inverse" \
$

This results in a public key of $e=7, n=33$ and a private key of $d=3$.

== Encryption

$
x &= 5 & "By question instructions" \
f(x) &= x^e mod n & "By definition" \
&= 5^7 mod 33 & "By substitution of" x, e, n \
&= 14 & "By arithmetic" \
$

== Decryption

$
f^(-1)(y) &= y^d mod n & "By definition" \
&= 14^3 mod 33 & "By substitution of" y, d, n \
&= 5 & "By arithmetic" \
$

#pagebreak()

= Question 3

#quote(block: true, quotes: true)[
	Build your public/private key pair and show how the plain text is encrypted using your public key and decrypted using your private key. Assume, plain text, $x=217$.
]

I choose the values $p=4111, q=5557$ from which to generate the keypair, and a value of $e=4777$, which satisfies $1 < e < phi (n) and gcd (x, phi (n)) = 1$.

$
p &= 4111 & "By choice" \
q &= 5557 & "By choice" \
n &= p q & "By definition" \
&= 22844827 & "By arithmetic" \
phi (n) &= (p-1)(q-1) & "By definition" \
&= (4111-1)(5557-1) & "By substitution of" p, q \
&= 22835160 & "By arithmetic" \
e &= 4777 & "By choice" \
1 &= (e d) mod phi (n) & "By definition" \
&= (4777 d) mod 22835160 & "By substitution of" e, phi (n) \
d &= 4421713 & "By modular multiplicative inverse" \
$

This results in a public key of $e=4777, n=22844827$ and a private key of $d=4421713$.

== Encryption

$
x &= 217 & "By question instructions" \
f(x) &= x^e mod n & "By definition" \
&= 217^4777 mod 22844827 & "By substitution of" x, e, n \
&= 18760882 & "By arithmetic" \
$

== Decryption

$
f^(-1)(y) &= y^d mod n & "By definition" \
&= 18760882^4421713 mod 22844827 & "By substitution of" y, d, n \
&= 217 & "By arithmetic" \
$

#pagebreak()

= Question 4

#quote(block: true, quotes: true)[
	Please develop a program that implements all the steps of the RSA algorithm. The program takes inputs of any two prime numbers and generates public-private key pairs. You can choose any programming language of your choice.
]

== Source

#raw(read("rsa.c"), lang: "C", tab-size: 4)

== Output

#figure(
	image("rsa_output.png", width: 100%),
	caption: [Screenshot of RSA keygen, encryption, and decryption output],
)

