/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 16:27:46 by tcali             #+#    #+#             */
/*   Updated: 2025/09/09 14:18:05 by tcali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLORS_H
# define COLORS_H
# ifndef PROMPT
// #  define PROMPT "\001\033[1;35m\002minishell$ \001\033[0;33m\002"
#  define PROMPT "minishell$ "
# endif
# ifndef C_MAGENTA
#  define C_MAGENTA "\033[0;35m"
# endif
# ifndef C_RED
#  define C_RED "\033[0;31m"
# endif
# ifndef C_BLUE
#  define C_BLUE "\033[0;34m"
# endif
# ifndef C_CYAN
#  define C_CYAN "\033[0;36m"
# endif
# ifndef C_YELLOW
#  define C_YELLOW "\033[0;33m"
# endif
# ifndef C_GREEN
#  define C_GREEN "\033[0;32m"
# endif
# ifndef C_MAGENTA_1
#  define C_MAGENTA_1 "\033[1;35m"
# endif
# ifndef C_RED_1
#  define C_RED_1 "\033[1;31m"
# endif
# ifndef C_BLUE_1
#  define C_BLUE_1 "\033[1;34m"
# endif
# ifndef C_CYAN_1
#  define C_CYAN_1 "\033[1;36m"
# endif
# ifndef C_YELLOW_1
#  define C_YELLOW_1 "\033[1;33m"
# endif
# ifndef C_GREEN_1
#  define C_GREEN_1 "\033[1;32m"
# endif
# ifndef C_RST
#  define C_RST "\033[0m"
# endif
#endif