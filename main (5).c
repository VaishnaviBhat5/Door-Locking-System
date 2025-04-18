#include <avr/io.h>
#include <util/delay.h>

#define lcdport PORTA
#define signal PORTB
#define en PB2
#define rw PB1
#define rs PB0

char password[4];  // Adjusted the size to 4 for the password
char lock[1];      // Adjusted the size to 1 for the lock
char key;

char keycheck();
char scankey();
void lcdcmd(unsigned char cmd);
void lcdint();
unsigned char lcd_read();
void lcddata(unsigned char data);
void send_a_command(unsigned char command);
void send_a_character(unsigned char character);
void send_a_string(char *string_of_characters);

int main()
{
    DDRA = 0xff;
    DDRB = 0x0f;
    DDRD = 0xf0;

    lcdint();

    lcddata('E');
    lcddata('N');
    lcddata('T');
    lcddata('E');
    lcddata('R');
    lcddata(' ');
    lcddata('P');
    lcddata('A');
    lcddata('S');
    lcddata('S');
    lcddata('W');
    lcddata('O');
    lcddata('R');
    lcddata('D');
    lcdcmd(0xC0);

    for (int i = 0; i < 4; i++)  // Changed to 4 for a 4-digit password
    {
        key = scankey();
        _delay_ms(1000);

        lcddata('');  // Display '' instead of the actual key pressed
        password[i] = key;
    }

    lcdcmd(0x01);

    if ((password[0] == '2') && (password[1] == '6') && (password[2] == '8') && (password[3] == '2'))  // Updated password check
    {
        lcddata('C');
        lcddata('O');
        lcddata('R');
        lcddata('R');
        lcddata('E');
        lcddata('C');
        lcddata('T');
        lcddata(' ');
        lcddata('P');
        lcddata('A');
        lcddata('S');
        lcddata('S');
        lcddata('W');
        lcddata('O');
        lcddata('R');
        lcddata('D');
        DDRC = 0xff;
        PORTC = 0x02;
        _delay_ms(3000); // Adjust this delay for motor opening time (anticlockwise)
        PORTC = 0x00;
        lcdcmd(0x01);
        lcdcmd(0x80);
        lcddata('L');
        lcddata('O');
        lcddata('C');
        lcddata('K');
        lcddata('?');
        lcddata(' ');
        lcddata('P');
        lcddata('R');
        lcddata('E');
        lcddata('S');
        lcddata('S');
        lcddata(' ');
        lcddata('1');
        lcdcmd(0xC0);

        key = scankey();
        _delay_ms(1000);

        lcddata(key);
        lock[0] = key;
        if (lock[0] == '1')
        {
            DDRC = 0xff;
            PORTC = 0x01;
            _delay_ms(3000); // Adjust this delay for motor closing time (clockwise)
            PORTC = 0x00;
            lcdcmd(0x01);
            lcddata('L');
            lcddata('O');
            lcddata('C');
            lcddata('K');
            lcddata('E');
            lcddata('D');
            _delay_ms(1000);
            return main();
        }
    }
    else
    {
        lcddata('W');
        lcddata('R');
        lcddata('O');
        lcddata('N');
        lcddata('G');
        lcddata(' ');
        lcddata('P');
        lcddata('A');
        lcddata('S');
        lcddata('S');
        lcddata('W');
        lcddata('O');
        lcddata('R');
        lcddata('D');
    }

    return main();
}

char scankey()
{
    char key = 'a';
    while (key == 'a')
    {
        key = keycheck();
    }
    return key;
}

char keycheck()
{
    PORTD = 0b11101111;
    _delay_ms(10);

    if ((PIND & 0b00000001) == 0) { return '7'; }
    if ((PIND & 0b00000010) == 0) { return '8'; }
    if ((PIND & 0b00000100) == 0) { return '9'; }
    if ((PIND & 0b00001000) == 0) { return 'A'; }

    PORTD = 0b11011111;
    _delay_ms(10);

    if ((PIND & 0b00000001) == 0) { return '4'; }
    if ((PIND & 0b00000010) == 0) { return '5'; }
    if ((PIND & 0b00000100) == 0) { return '6'; }
    if ((PIND & 0b00001000) == 0) { return 'B'; }

    PORTD = 0b10111111;
    _delay_ms(10);

    if ((PIND & 0b00000001) == 0) { return '1'; }
    if ((PIND & 0b00000010) == 0) { return '2'; }
    if ((PIND & 0b00000100) == 0) { return '3'; }
    if ((PIND & 0b00001000) == 0) { return 'C'; }

    PORTD = 0b01111111;
    _delay_ms(10);

    if ((PIND & 0b00000001) == 0) { return '*'; }
    if ((PIND & 0b00000010) == 0) { return '0'; }
    if ((PIND & 0b00000100) == 0) { return '#'; }
    if ((PIND & 0b00001000) == 0) { return 'D'; }

    return 'a';
}

void lcdint()
{
    lcdcmd(0x38);
    _delay_ms(1);
    lcdcmd(0x01);
    _delay_ms(1);
    lcdcmd(0x0F);
    _delay_ms(100);
    lcdcmd(0x82);
    _delay_ms(100);
    lcdcmd(0x89);
    _delay_ms(10);
    lcdcmd(0x01);
}

void lcdcmd(unsigned char x)
{
    lcdport = x;
    signal = (0 << rs) | (0 << rw) | (1 << en);
    _delay_ms(1);
    signal = (0 << rs) | (0 << rw) | (0 << en);
    _delay_ms(50);
}

void lcddata(unsigned char data)
{
    lcdport = data;
    signal = (1 << rs) | (0 << rw) | (1 << en);
    _delay_ms(1);
    signal = (1 << rs) | (0 << rw) | (0 << en);
    _delay_ms(50);
}

unsigned char lcd_read()
{
    signal = (1 << rs) | (1 << rw) | (1 << en);
    _delay_ms(1);
    signal = (1 << rs) | (1 << rw) | (0 << en);
    _delay_ms(50);
}
