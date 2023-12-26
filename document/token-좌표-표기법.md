# toeken 좌표 표기법

### 배경
모든 token 등은 고유한 좌표를 가짐. 이는 나중 오류 처리를 위함임. 예컨대 structure Token은 아래와 같은 좌표값을 포함함.

```
  size_t col;
  size_t col_first;
  size_t row;
  size_t row_char;
  size_t row_char_first;
```
본 문서는 해당 좌표 체계를 설명함.
- 짜냐 짜냐 짜냐 짜냐 짜냐 col\_first: y좌표 시작이라 생각하짜.
예컨대 문자열은 여러줄일 수 있기에 처음과 끝이 나뉨.
- col: y좌표의 끝.
- row\_char\_first: x좌표의 시작
- row\_char: x좌표의 끔

### 예
본 문서에서는 아래와 같은 예를 사용하기로 함.
```
\hello\hello

(,) %hello
( , )

) (     * *
-+-+
```
tokenize하면 첫 토큰은 '\hello'가 되어야함(\는 새로운 시작... $\LaTeX$을 따르리).
좌표의 시작은 $(0, 0)$임.
따라서 첫 토큰 '\hello'의 경우 col\_first와 row\_char\_first가 0임.
시작과 끝이 같지 않기를 원했음.
즉, row의 경우 끝나는 글자 번째 숫자를 col\_char에 넣어야함.
예컨대, 'a'는 $(0, 0)$에서 시작하면, $(1, 0)$에서 끝남.

즉, 첫 토큰인 '\hello'는...
- col\_first: 0
- col: 0
- row\_char\_first: 0
- row\_char: 6

다른 예컨대...
value: ','
- col\_first: 2
- col: 2
- row\_char\_first: 1
- row\_char: 2
