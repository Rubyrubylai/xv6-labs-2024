#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"
#define MAX_LEN 100

int main(int argc, char *argv[]) {
	char *command = argv[1];
	char bf;
	char paramv[MAXARG][MAX_LEN]; // 是一個二維陣列，一塊大小為 MAXARG x MAX_LEN 的連續記憶體
	char *m[MAXARG];

	while (1) {
		memset(paramv, 0, MAXARG * MAX_LEN);
		// 將 xargs 後面的參數存入 paramv
		for (int i=1; i<argc; i++) {
			strcpy(paramv[i-1], argv[i]);
		}

		int cursor = 0; // 目前在字串中「寫到哪個位置」
		int flag = 0; // 是否目前正在讀一個字串（1 表示正在讀，0 表示剛遇到空白）
		int read_result;
		int count = argc-1; // 才不會覆蓋掉 xargs 後面的參數
		// 找 xargs 前面的參數
		while (((read_result = read(0, &bf, 1))) > 0 && bf != '\n') { // 從標準輸入（fd 0）讀取 1 個位元組放到變數 bf 中，直到遇到換行字元 \n
			if (bf == ' ' && flag == 1) { // 如果這個字元是空白 ' '，而且剛剛有讀到字元（flag 為 1），表示剛好一個參數結束了
				count++;
				cursor = 0;
				flag = 0;
			}
			else if (bf != ' ') { // 如果不是空白，就把這個字元放進 paramv[count] 這一行的字串中，表示正在組字串
				paramv[count][cursor++] = bf;
				flag = 1;
			}
		}

		// encounters EOF of input or \n
		if (read_result <= 0) {
			break;
		}

		for (int i=0; i<MAXARG-1; i++) {
			m[i] = paramv[i];
		}
		m[MAXARG-1] = 0;

		if (fork() == 0) {
			exec(command, m); // command 是要執行的指令，例如 "echo" 或 "grep"，m 是傳給這個指令的參數，例如 {"echo", "hello", "world", 0}
			exit(0);
		} else {
			wait((int *) 0);
		}
	}
	exit(0);
}