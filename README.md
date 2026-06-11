# Plus++ Lexical Analyzer in C

## English

This repository contains a lexical analyzer written in C for a small custom programming language called Plus++.

The program reads a `.plus` source file, analyzes it character by character, identifies valid lexical tokens, writes the token list into a `.lx` output file, and reports lexical errors with line numbers. The analyzer also maintains a simple symbol table to track declared variables and detect undeclared or redeclared identifiers.

The project was built to understand how the first stage of a compiler works: reading raw source code and converting it into meaningful tokens before parsing or execution.

---

## Project Overview

The lexical analyzer processes Plus++ source code and classifies language elements such as:

* Keywords
* Identifiers
* Integer constants
* String constants
* Operators
* Block symbols
* End-of-line symbols
* Comments

For example, a Plus++ source file can contain variable declarations, assignments, repeat blocks, write statements, comments, and string outputs. The analyzer converts these elements into token representations and saves them in a separate `.lx` file.

---

## Supported Plus++ Language Elements

### Keywords

The analyzer recognizes the following reserved keywords:

```text
number
write
repeat
times
newline
and
```

### Identifiers

Identifier rules:

* Must start with a letter
* Can contain letters, digits, and underscores
* Cannot start with an underscore
* Cannot exceed 20 characters
* Must be declared before use
* Cannot be declared more than once

### Operators

Supported operators:

```text
:=
+=
-=
```

### Constants

The analyzer supports:

* Integer constants
* Negative integer constants
* String constants enclosed in double quotes

Real numbers, numbers with a plus sign, and invalid number formats are reported as lexical errors.

### Blocks and Statements

The analyzer recognizes:

```text
{
}
;
```

* `{` opens a block
* `}` closes a block
* `;` marks the end of a statement

Repeat statements must be followed by a block.

### Comments

Comments are written between `*` symbols on the same line.

Example:

```text
* this is a comment *
```

If a comment is not closed before the end of the line, the analyzer reports an error.

---

## Example Input

```text
number size;
number sum;
size:=5;

repeat size times
{
  write size and newline;
  sum+=size;
}

write newline and "Sum:" and sum;
```

---

## Example Output

```text
Keyword(number)
Identifier(size)
EndOfLine
Keyword(number)
Identifier(sum)
EndOfLine
Identifier(size)
Operator(:=)
IntConstant(5)
EndOfLine
Keyword(repeat)
Identifier(size)
Keyword(times)
OpenBlock
Keyword(write)
Identifier(size)
Keyword(and)
Keyword(newline)
EndOfLine
Identifier(sum)
Operator(+=)
Identifier(size)
EndOfLine
CloseBlock
Keyword(write)
Keyword(newline)
Keyword(and)
StringConstant("Sum:")
Keyword(and)
Identifier(sum)
EndOfLine
```

---

## Error Handling

The analyzer detects and reports several lexical errors, including:

* Identifier starting with a digit
* Identifier starting with an underscore
* Identifier longer than 20 characters
* Redeclared variables
* Undeclared variable usage
* Unclosed comments
* Unclosed string literals
* Invalid number formats
* Real number usage
* Invalid operators
* Unexpected characters
* Missing `{` after `repeat ... times`
* Extra closing block `}`
* Unclosed blocks

Each error message includes the line number where the problem occurs.

---

## Technologies Used

* C
* GCC / C compiler
* File I/O
* Character-by-character lexical analysis
* Symbol table logic
* Token classification
* Error handling with line tracking

---

## How to Run

Compile the lexical analyzer:

```bash
gcc la.c -o la
```

Run it with a Plus++ source file name without the `.plus` extension:

```bash
./la myscript
```

The program automatically reads:

```text
myscript.plus
```

and creates:

```text
myscript.lx
```

---

## What I Implemented and Learned

In this project, I implemented the core logic of a lexical analyzer manually in C.

I worked on reading a source file character by character, separating meaningful language elements, classifying tokens, validating language-specific rules, and writing the output into a token file. This helped me understand how source code is processed before syntax analysis.

I also implemented a simple symbol table to track declared variables. With this structure, the analyzer can detect duplicate declarations and undeclared variable usage. This made the relationship between lexical analysis and semantic checks clearer.

The error handling part helped me practice detecting invalid input patterns and reporting useful messages with line numbers. Instead of only accepting correct input, the analyzer also handles incorrect identifiers, invalid constants, unclosed strings, unclosed comments, and block-related mistakes.

Main topics I practiced:

* File reading and output file generation in C
* Character-level source code analysis
* Token recognition and classification
* Keyword and operator detection
* Identifier validation
* Integer and string constant handling
* Comment handling
* Symbol table implementation
* Lexical error detection
* Line-number-based error reporting
* Building a compiler-related tool from scratch

---

# C ile Plus++ Lexical Analyzer

## Türkçe

Bu repository, Plus++ adlı küçük özel bir programlama dili için C diliyle yazılmış bir lexical analyzer içermektedir.

Program, `.plus` uzantılı kaynak dosyayı okur, karakter karakter analiz eder, geçerli lexical tokenları belirler, token listesini `.lx` uzantılı çıktı dosyasına yazar ve lexical hataları satır numarasıyla birlikte raporlar. Analyzer ayrıca tanımlanan değişkenleri takip etmek için basit bir symbol table yapısı kullanır. Bu sayede tanımlanmamış veya tekrar tanımlanmış identifier hataları yakalanabilir.

Bu proje, derleyicilerin ilk aşamasında kaynak kodun nasıl okunduğunu ve parser aşamasından önce nasıl anlamlı tokenlara ayrıldığını anlamak için geliştirildi.

---

## Proje Özeti

Lexical analyzer, Plus++ kaynak kodunu işler ve dil elemanlarını sınıflandırır:

* Keyword
* Identifier
* Integer constant
* String constant
* Operator
* Block sembolleri
* Satır sonu sembolleri
* Comment

Örneğin bir Plus++ kaynak dosyasında değişken tanımlama, atama, repeat bloğu, write komutu, comment ve string output bulunabilir. Analyzer bu yapıları tokenlara dönüştürür ve ayrı bir `.lx` dosyasına yazar.

---

## Desteklenen Plus++ Dil Elemanları

### Keywordler

Analyzer aşağıdaki reserved keywordleri tanır:

```text
number
write
repeat
times
newline
and
```

### Identifier Kuralları

Identifier kuralları:

* Harf ile başlamalıdır
* Harf, rakam ve alt çizgi içerebilir
* Alt çizgi ile başlayamaz
* 20 karakterden uzun olamaz
* Kullanılmadan önce tanımlanmalıdır
* Aynı değişken birden fazla kez tanımlanamaz

### Operatörler

Desteklenen operatörler:

```text
:=
+=
-=
```

### Sabitler

Analyzer şu sabitleri destekler:

* Integer constant
* Negatif integer constant
* Çift tırnak içine alınmış string constant

Real number, başında `+` bulunan sayı ve hatalı sayı formatları lexical error olarak raporlanır.

### Bloklar ve Satır Sonları

Analyzer şu sembolleri tanır:

```text
{
}
;
```

* `{` blok başlangıcını gösterir
* `}` blok bitişini gösterir
* `;` statement sonunu gösterir

`repeat ... times` yapısından sonra blok açılması beklenir.

### Comment Yapısı

Commentler aynı satırda `*` sembolleri arasına yazılır.

Örnek:

```text
* this is a comment *
```

Comment satır bitmeden kapatılmazsa analyzer hata raporlar.

---

## Örnek Girdi

```text
number size;
number sum;
size:=5;

repeat size times
{
  write size and newline;
  sum+=size;
}

write newline and "Sum:" and sum;
```

---

## Örnek Çıktı

```text
Keyword(number)
Identifier(size)
EndOfLine
Keyword(number)
Identifier(sum)
EndOfLine
Identifier(size)
Operator(:=)
IntConstant(5)
EndOfLine
Keyword(repeat)
Identifier(size)
Keyword(times)
OpenBlock
Keyword(write)
Identifier(size)
Keyword(and)
Keyword(newline)
EndOfLine
Identifier(sum)
Operator(+=)
Identifier(size)
EndOfLine
CloseBlock
Keyword(write)
Keyword(newline)
Keyword(and)
StringConstant("Sum:")
Keyword(and)
Identifier(sum)
EndOfLine
```

---

## Hata Yönetimi

Analyzer aşağıdaki lexical hataları yakalar ve raporlar:

* Rakamla başlayan identifier
* Alt çizgiyle başlayan identifier
* 20 karakterden uzun identifier
* Tekrar tanımlanan değişken
* Tanımlanmadan kullanılan değişken
* Kapatılmamış comment
* Kapatılmamış string literal
* Hatalı sayı formatları
* Real number kullanımı
* Geçersiz operatör
* Beklenmeyen karakter
* `repeat ... times` sonrasında eksik `{`
* Fazladan kapanış bloğu `}`
* Kapatılmamış bloklar

Her hata mesajında hatanın oluştuğu satır numarası gösterilir.

---

## Kullanılan Teknolojiler

* C
* GCC / C compiler
* Dosya okuma ve yazma
* Karakter karakter lexical analysis
* Symbol table mantığı
* Token sınıflandırma
* Satır takibi ile hata yönetimi

---

## Çalıştırma

Lexical analyzer'ı derlemek için:

```bash
gcc la.c -o la
```

`.plus` uzantısını yazmadan dosya adıyla çalıştırmak için:

```bash
./la myscript
```

Program otomatik olarak şu dosyayı okur:

```text
myscript.plus
```

ve şu çıktı dosyasını oluşturur:

```text
myscript.lx
```

---

## Bu Projede Ne Uyguladım ve Ne Öğrendim?

Bu projede C diliyle bir lexical analyzer'ın temel çalışma mantığını manuel olarak implemente ettim.

Kaynak dosyayı karakter karakter okuma, anlamlı dil elemanlarını ayırma, tokenları sınıflandırma, dile özel kuralları doğrulama ve çıktıyı token dosyasına yazma işlemlerini uyguladım. Bu süreç, kaynak kodun syntax analysis aşamasından önce nasıl işlendiğini anlamamı sağladı.

Ayrıca tanımlanan değişkenleri takip etmek için basit bir symbol table yapısı oluşturdum. Bu yapı sayesinde analyzer, tekrar değişken tanımlama ve tanımlanmamış değişken kullanımı gibi durumları yakalayabiliyor. Bu da lexical analysis ile bazı temel semantic kontroller arasındaki ilişkiyi daha net görmemi sağladı.

Hata yönetimi kısmında, yalnızca doğru girdiyi işlemek yerine hatalı inputları da yakalamayı ve satır numarasıyla anlamlı hata mesajları üretmeyi uyguladım. Hatalı identifierlar, geçersiz sabitler, kapatılmamış stringler, kapatılmamış commentler ve blok hataları bu kapsamda ele alındı.

Bu projede pratik yaptığım ana konular:

* C ile dosya okuma ve çıktı dosyası üretme
* Karakter seviyesinde kaynak kod analizi
* Token tanıma ve sınıflandırma
* Keyword ve operator kontrolü
* Identifier doğrulama
* Integer ve string constant işleme
* Comment işleme
* Symbol table implementasyonu
* Lexical error detection
* Satır numarasına göre hata raporlama
* Sıfırdan compiler-related bir araç geliştirme
