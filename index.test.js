const systemFontPaths = require('.');

// add a custom matcher that expects unique string arrays
expect.extend({
    toBeArrayOfUniqueStrings(received) {
        if (!Array.isArray(received)) {
            return {
                pass: false,
                message: () => (
                    this.utils.matcherHint('toBeArrayOfUniqueStrings', 'received', '')
                    + `\n\n${this.utils.RECEIVED_COLOR('received')} value must be an array\n\n`
                    + this.utils.printWithType('Received', received, this.utils.printReceived)
                ),
            };
        }
        const errors = received.reduce((acc, value, index, array) => {
            const pass = typeof value === 'string' && array.indexOf(value) === index;
            if (!pass) acc.push(index);
            return acc;
        }, []);
        return {
            pass: errors.length === 0,
            message: () => {
                const lines = [];
                let last = 0;
                for (const idx of errors) {
                    const count = idx - last;
                    if (count) {
                        if (last) lines.push(this.utils.DIM_COLOR(`    "${received[last]}",`));
                        if (count > 2) lines.push(this.utils.DIM_COLOR(`    ...${count - 2}...`));
                        if (count > 1) lines.push(this.utils.DIM_COLOR(`    "${received[idx - 1]}",`));
                    }
                    lines.push(
                        this.utils.RECEIVED_COLOR('+   ')
                        + this.utils.printReceived(received[idx])
                        + this.utils.RECEIVED_COLOR(','),
                    );
                    last = idx + 1;
                }
                if (received.length > last) {
                    if (last) lines.push(this.utils.DIM_COLOR(`    "${received[last]}",`));
                    if ((received.length - last) > 1) {
                        lines.push(this.utils.DIM_COLOR(`    ...${received.length - last - 1}...`));
                    }
                }
                return this.utils.matcherHint('toBeArrayOfUniqueStrings', 'received', '')
                    + `\n\n  ${this.utils.DIM_COLOR('Array [')}`
                    + lines.map((l) => `\n${l}`).join('')
                    + `\n  ${this.utils.DIM_COLOR(']')}`;
            },
        };
    },
});

test('module exports a function', () => {
    expect(typeof systemFontPaths).toBe('function');
});

test('function returns an array of unique strings', async () => {
    await expect(systemFontPaths()).resolves.toBeArrayOfUniqueStrings();
}, 20000);