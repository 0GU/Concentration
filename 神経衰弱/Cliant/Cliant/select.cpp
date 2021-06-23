#include "main.h"

IPDATA IP_set() {
	IPDATA ip;

	while (1) {
		ClearDrawScreen();

		DrawString(IP_INPUT_POS_X, IP_INPUT_POS_Y, "IP“ü—Í", GetColor(WHITE));

		ip.d1 = KeyInputNumber(IP_KEY_INPUT_POS_X, IP_KEY_INPUT_POS_Y, IP_KEY_INPUT_MAX_NUM, IP_KEY_INPUT_MIN_NUM, FALSE);
		ip.d2 = KeyInputNumber(IP_KEY_INPUT_POS_X, IP_KEY_INPUT_POS_Y, IP_KEY_INPUT_MAX_NUM, IP_KEY_INPUT_MIN_NUM, FALSE);
		ip.d3 = KeyInputNumber(IP_KEY_INPUT_POS_X, IP_KEY_INPUT_POS_Y, IP_KEY_INPUT_MAX_NUM, IP_KEY_INPUT_MIN_NUM, FALSE);
		ip.d4 = KeyInputNumber(IP_KEY_INPUT_POS_X, IP_KEY_INPUT_POS_Y, IP_KEY_INPUT_MAX_NUM, IP_KEY_INPUT_MIN_NUM, FALSE);

		DrawFormatString(IP_DIS_POS_X, IP_DIS_POS_Y, GetColor(WHITE),
			"IP:%d:%d:%d:%d",
			ip.d1,
			ip.d2,
			ip.d3,
			ip.d4
		);
		DrawString(IP_CONFIRMATION_POS_X, IP_CONFIRMATION_POS_Y, "‚±‚ê‚Å—Ç‚¢‚Å‚·‚©H", GetColor(WHITE));
		DrawString(IP_ANSWER_POS_X, IP_ANSWER_POS_Y, "0...‚¢‚¢‚¦ 1...‚Í‚¢\n>>", GetColor(WHITE));
		if (KeyInputNumber(IP_KEY_INPUT_ANSWER_POS_X, IP_KEY_INPUT_ANSWER_POS_Y, 1, 0, FALSE))break;
	}
	return ip;
}