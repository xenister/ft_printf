/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_start_processing.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susivagn <susivagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/07 13:42:34 by susivagn          #+#    #+#             */
/*   Updated: 2017/03/29 18:21:08 by susivagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		ft_start_processing(char c)
{
	if (g_flags.flagwidth != 0 || g_flags.flagzero != -1)
		g_flags.lm = g_flags.flagwidth >= g_flags.flagzero ? g_flags.flagwidth :
			g_flags.flagzero;
	if (c == 's' || c == 'S' || c == 'c' || c == 'C')
		ft_string_char(c);
	if (c == 'd' || c == 'i' || c == 'D')
		ft_signed_numbers(c);
	if (c == 'o' || c == 'O' || c == 'u' || c == 'U' || c == 'x' || c == 'X')
		ft_unsigned_numbers(c);
	/*if (c == 'p')
		ft_print_adress();*/
	return (0);
}

intmax_t	ft_process_lenmod_signed()
{
	intmax_t	nbr;

	if (g_len_modifier.modhh == 1 || g_len_modifier.modh == 1 ||
		g_len_modifier.modl == 1)
	{
		nbr = (int)va_arg(g_vl, void*);
		if (g_len_modifier.modhh == 1)
			nbr = (char)nbr;
		else if (g_len_modifier.modh == 1)
			nbr = (short)nbr;
		else if (g_len_modifier.modl == 1)
			nbr = nbr;
		return (nbr);
	}
	else if (g_len_modifier.modll == 1)
		nbr = (long long)va_arg(g_vl, void*);
	else if (g_len_modifier.modj == 1)
		nbr = (intmax_t)va_arg(g_vl, void*);
	else if (g_len_modifier.modz == 1)
		nbr = (size_t)va_arg(g_vl, void*);
	else
		nbr = (int)va_arg(g_vl, void*);
	return (nbr);
}

intmax_t	ft_process_lenmod_signed_big()
{
	intmax_t	nbr;

	if (g_len_modifier.modhh == 1 || g_len_modifier.modh == 1 ||
		g_len_modifier.modl == 1)
		return ((intmax_t)va_arg(g_vl, void*));
	else if (g_len_modifier.modll == 1)
		nbr = (long long)va_arg(g_vl, void*);
	else if (g_len_modifier.modj == 1)
		nbr = (intmax_t)va_arg(g_vl, void*);
	else if (g_len_modifier.modz == 1)
		nbr = (size_t)va_arg(g_vl, void*);
	else
		nbr = (intmax_t)va_arg(g_vl, void*);
	return (nbr);
}

uintmax_t	ft_process_lenmod_unsigned()
{
	uintmax_t	nbr;

	if (g_len_modifier.modhh == 1)
		nbr = (unsigned char)va_arg(g_vl, void*);
	else if (g_len_modifier.modh == 1)
		nbr = (unsigned short)va_arg(g_vl, void*);
	else if (g_len_modifier.modl == 1)
		nbr = (unsigned long)va_arg(g_vl, void*);
	else if (g_len_modifier.modll == 1)
		nbr = (unsigned long long)va_arg(g_vl, void*);
	else if (g_len_modifier.modj == 1)
		nbr = (uintmax_t)va_arg(g_vl, void*);
	else if (g_len_modifier.modz == 1)
		nbr = (size_t)va_arg(g_vl, void*);
	else
		nbr = (va_arg(g_vl, uintmax_t));
	return (nbr);
}

void	ft_unsigned_numbers(char c)
{
	uintmax_t	nbr;

	nbr = ft_process_lenmod_unsigned();
	if (g_flags.flagplus != 0)
		g_flags.flagplus = 0;
	if (g_flags.flaghtag != 0)
		g_flags.flaghtag = (c == 'x') ? 1 : 2;
	else if (c == 'o' || c == 'O')
		g_flags.flaghtag = 3;
	g_buff = conv_hexa(nbr, c);
	if (g_flags.flagprecision != -1 && (g_flags.flagzero = -1))
	{
		ft_process_precision_nbr(g_buff);
		ft_place_htag_pres(g_flags.flaghtag);
	}
	if (g_flags.flaghtag != 0)
		ft_place_htag(g_flags.flaghtag, 0);
	ft_process_flag_str(ft_strlen(g_buff));
}

void	ft_signed_numbers(char c)
{
	if (c == 'd' || c == 'i')
		g_buff = ft_itoa(ft_process_lenmod_signed());
	else
		g_buff = ft_itoa(ft_process_lenmod_signed_big());
	if (ft_strchr(g_buff, '-') && g_flags.flagplus == 0 &&
		(g_flags.flagplus = -1))
		if (g_buff)
			g_buff = ft_strdup(&g_buff[1], 0);
	if (g_flags.flagspace == 1)
		g_buff = ft_morealloc(g_buff, 1, 1);
	if (g_flags.flagprecision != -1 && (g_flags.flagzero = -1))
		ft_process_precision_nbr(g_buff);
	ft_process_flag_str(ft_strlen(g_buff));
	ft_place_sign(g_flags.flagplus);
}

void	ft_process_precision_nbr(char *str)
{
	int		lenstr;
	int		sizeprecision;

	lenstr = ft_strlen(str);
	if (g_flags.flagprecision > lenstr)
	{
		sizeprecision = lenstr + (g_flags.flagprecision - lenstr);
		g_buff = ft_memalloc((sizeof(char) * sizeprecision + 1), ' ');
		g_buff[sizeprecision] = '\0';
		sizeprecision = g_flags.flagprecision - lenstr;
		ft_str_fill_nchar_lr(g_buff, '0', sizeprecision);
		ft_place_in_str(&g_buff[sizeprecision], str, 0);
		ft_place_sign_pres(g_flags.flagplus);
	}
	else if (g_flags.flagprecision <= lenstr)
	{
		g_buff = str;
		ft_place_sign(g_flags.flagplus);
	}
}

void	ft_place_htag_pres(int sign)
{
	if (sign == 1 || sign == 2)
	{
		g_buff = ft_morealloc(g_buff, 2 , 1);
		assign_htag(g_buff, sign);
		g_flags.flaghtag = 0;
	}
	if (sign == 3)
	{
		g_buff = ft_morealloc(g_buff, 1 ,1);
		assign_htag(g_buff, sign);
		g_flags.flaghtag = 0;
	}
}

void	ft_place_htag(int sign, int i)
{
	while (g_buff[i] == ' ' && g_buff[i])
		i++;
	if (g_buff[i] == '0' && g_buff[i + 1] == '0' && (sign == 1 || sign == 2))
		g_buff[i + 1] = (sign == 1) ? 'x' : 'X';
	else if (i > 0 && ft_isallhexa(&g_buff[i]) && sign == 3)
		g_buff[i - 1] = '0';
	else if (i > 1 && ft_isallhexa(&g_buff[i]) && (sign == 1 || sign == 2))
	{
		g_buff[i - 2] = '0';
		g_buff[i - 1] = (sign == 1) ? 'x' : 'X';
	}
	else if (ft_isallhexa(g_buff))
	{
		if (sign == 1 || sign == 2)
		{
			g_buff = ft_morealloc(g_buff, 2, 1);
			g_buff[0] = '0';
			g_buff[1] = (sign == 1) ? 'x' : 'X';
		}
		if (sign == 3)
		{
			g_buff = ft_morealloc(g_buff, 1, 1);
			g_buff[0] = '0';
		}
	}
}

void	assign_htag(char *src, int sign)
{
	if (sign == 1 || sign == 2)
	{
		g_buff[0] = '0';
		g_buff[1] = (sign == 1) ? 'x' : 'X';
	}
	if (sign == 3)
		g_buff[0] = '0';
}

void	ft_place_sign_pres(int sign)
{
	if (sign != 0)
	{
		g_buff = ft_morealloc(g_buff, 1, 1);
		if (sign == -1)
			g_buff[0] = '-';
		else if (sign == 1)
			g_buff[0] = '+' ;
	}
	g_flags.flagplus = 0;
}

void	ft_place_sign(int sign)
{
	int		i;

	i = 0;
	if (sign != 0)
	{
		while (g_buff[i] == ' ' && g_buff[i])
			i++;
		if (g_buff[i] == '0' && sign == -1)
			g_buff[i] = '-';
		else if (g_buff[i] == '0' && sign == 1)
			g_buff[i] = '+';
		else if (i > 0 && g_buff[i] >= '0' && g_buff[i] <= '9')
			g_buff[i - 1] = (sign < 0) ? '-' : '+';
		else if (ft_isalldigit(g_buff))
		{
			g_buff = ft_morealloc(g_buff, 1, 1);
			g_buff[i] = (sign < 0) ? '-' : '+';
		}
	}
}

void	ft_process_precision_str(char *str, char c)
{
	int		lenstr;

	lenstr = ft_strlen(str);
	if (g_flags.flagprecision >= lenstr)
		g_buff = str;
	else if (g_flags.flagprecision == 0 && (c == 'c' || c == 'C'))
	{
		g_flags.flagprecision = 1;
		g_buff = ft_strsub(str, 0, g_flags.flagprecision);
	}
	else if (g_flags.flagprecision == 0 && (c != 'c' || c != 'C'))
		g_buff = ft_strsub(str, 0, 0);
	else
		g_buff = ft_strsub(str, 0, g_flags.flagprecision);
}

void	ft_string_char(char c)
{
	int		i;
	char	*cpystr;

	i = 0;
	if (c == 'c')
		ft_char();
	if (c == 's')
	{
		cpystr = ft_strdup((char*)va_arg(g_vl, void*), 0);
		if (g_flags.flagprecision != -1)
			ft_process_precision_str(cpystr, c);
		else
			g_buff = ft_strdup(cpystr, 1);
		ft_process_flag_str(ft_strlen(g_buff));
	}
}

void	ft_process_flag_str(int lenstr)
{
	char	*buff;
	int		len;

	if (g_flags.lm >= lenstr)
	{
		len = g_flags.lm >= lenstr ? g_flags.lm : lenstr;
		buff = (char*)ft_memalloc((sizeof(char) * len), ' ');
		buff[len] = '\0';
		if (g_flags.flagminus == 0 && g_flags.flagzero != -1)
		{
			ft_str_fill_nchar_rl(buff, '0', g_flags.flagzero);
			g_buff = ft_place_in_str(buff, g_buff , 1);
		}
		else if (g_flags.flagminus == 1)
			g_buff = ft_place_in_str(buff, g_buff, 0);
		else
			g_buff = ft_place_in_str(buff, g_buff, 1);
	}
}

void	ft_char()
{
	g_big_buff = (wchar_t)malloc(sizeof(wchar_t*) * 2);
	g_big_buff[1] = '\0';
	g_big_buff[0] = (va_arg(g_vl, wchar_t));
		ft_process_precision_str(g_buff, 'c');
	ft_process_flag_str(ft_strlen(g_buff));
}