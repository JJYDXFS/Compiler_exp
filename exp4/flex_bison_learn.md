# win下学习flex+yacc

```shell
flex xxx.l
cc lex.yy.c -L /thePathOfLibfl.a -lfl
# thePathOfLibfl.a -> G:\GnuWin32\lib
```

在cmd运行程序，Ctrl+C结束程序

## 使用bison制定语法

### 对于优先级的问题

显式定义和隐式定义

**隐式定义**：对不同的优先级使用不同的非终结符

```
// EOL 代表回车
list->list exp EOL	// 动作：输出结果
	->list EOL	// 输出匹配符

exp->factor
   ->exp '+' factor
   ->exp '-' factor
   
factor->term
      ->factor '*' term
      ->factor '/' term

term->NUMBER
	->'|' term
	->'(' exp ')'
	->'-' term
```



```bison
list: /* 空 */
   | list exp EOL {
	printf("= %1.4g\n",eval($2));
	freeTree($2);
	printf("> ");
}

| list EOL { printf("> "); }	/* 匹配空行 */
;

exp: factor
   | exp '+' factor { $$ = ntree('+', $1, $3); }
   | exp '-' factor { $$ = ntree('-', $1, $3); }
;

factor: term
   | factor '*' term { $$ = ntree('*', $1, $3); }
   | factor '/' term { $$ = ntree('/', $1, $3); }
;

term: NUMBER { $$ = nnum($1); }
    | '|' term { $$ = ntree('|', $2, NULL); }
    | '(' exp ')' { $$ = $2; }
    | '-' term { $$ = ntree('M', $2, NULL);}
;

```

显式定义：使用bison提供的 

%left	左结合

%right	右结合

%noassoc	没有结合性

出现的顺序，从低到高

```
'-' exp %prec UMINUS{ $$ = newast('M', NULL, $2)}	// 把UMINUS的优先级赋给'-'
```

