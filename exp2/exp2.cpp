char Precede(char a, char b)
{
	//判断符号优先级的函数
	switch (a)
	{
		case'+':
		{
			if (b == '+' || b == '-' || b == ')' || b == '#')
				return '>';
			else return '<';
			break;
		}
		case'-':
		{
			if (b == '+' || b == '-' || b == ')' || b == '#')
				return '>';
			else return '<';
			break;
		}
		case'*':
		{
			if (b == '(')
				return '<';
			else return '>';
			break;
		}
		case'/':
		{
			if (b == '(')
				return '<';
			else return '>';
			break;
		}
		case'(':
		{
			if (b == ')')
				return '=';
			else return '<';
			break;
		}
		case')':
		{
			return '>';
			break;
		}
		case'#':
		{
			if (b == '#')
				return '=';
			else return '<';
			break;
		}
	}
}									