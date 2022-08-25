# syntax

<sth> : a code or an expression
sth   : a keyword
()*   : an expression that can repeats
()?   : an expression that can be removed
()|() : one of the expressions must be chosen
()@   : in the expression, order isn't important

# single-line if statement

if <condition>: <body>
(elif <condition>: <body>?*
(else: <body>)?

# single-line if-else statement

<condition> ? <true-body> : <false-body>

# multi-line if statement

if <condition>
{
    <body>
}
(elif <condition>
{
    <body>
})?*
(else
{
    <body>
})?

# switch statement

switch <expression>
{
    (case <check>: <body> (break)?)*
    (default: <body>)?
}

# single-line for statement

for <var> = (<start>)? to <end> (step <step>)?(: <body>)?

## statements

continue (<condition>)?
break (<condition>)?

# multi-line for statement

for <var> = (<start>)? to <end> (step <step>)?
{
    <body>
}

## statements

continue (<condition>)?
break (<condition>)?

# single-line while statement

while (<condition>)?(: <body>)?

## statements

continue (<condition>)?
break (<condition>)?

# multi-line while statement

while (<condition>)?
{
    <body>
}

## statements

continue (<condition>)?
break (<condition>)?

# single-line do-while statement

do: <body> while (<condition>)?

## statements

continue (<condition>)?
break (<condition>)?

# multi-line do-while statement

do
{
    <body>
} while (<condition>)?

## statements

continue (<condition>)?
break (<condition>)?

# single-line loop statement

loop (<init> (, <init>)?*)?(; <condition> (, <condition>)?*)?(; <step> (, <step>)?*)?(: <body>)?

## statements

continue (<condition>)?
break (<condition>)?

# multi-line loop statement

loop (<init> (, <init>)?*)?(; <condition> (, <condition>)?*)?(; <step> (, <step>)?*)?
{
    <body>
}

## statements

continue (<condition>)?
break (<condition>)?

# single-line try statement

try: <body>
(except <error>: <body>)?*
(except: <body>)?

# multi-line try statement

try
{
    <body>
}
(except <error>
{
    <body>
})?*
(except
{
    <body>
})?

# import statement

import <library>

# var assign statement

(var)? (((public)|(private))? ((global)|(local))? (const)? (static)?)@ <name> <assign-operator> <value>

## special case

(var)? (((public)|(private))? ((global)|(local))? (const)? (static)?)@ <name> (++)|(--)

# var access statement

<name>

# single-line function definition statement

func ((((public)|(private))? ((global)|(local))? (const)? (static)?)@ <name>)? ((<var> (, <var>)?*)?): <body>

## statements

return (<return-value>)?

# multi-line function definition statement

func ((((public)|(private))? ((global)|(local))? (const)? (static)?)@ <name>)? ((<var> (, <var>)?*)?)
{
    <body>
}

## statements

return (<return-value>)?

# function call statement

<name>((<value> (, <value>)?*)?)

# dollar-function call statement

($)<name> (<arg> (, <arg>)?*)?
