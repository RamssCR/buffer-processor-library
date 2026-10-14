import { createRequire } from 'node:module'
const require = createRequire(import.meta.url)

const { processBuffer } = require('./build/Release/NativeModule.node')

const originalBuffer = Buffer.from("Hello from Node.js native C++ world!", "utf-8");
const xorKey = 0xAA;

console.time("C++ Native Processing");
const result = processBuffer(originalBuffer, xorKey);
console.timeEnd("C++ Native Processing");

console.log("Original String:", originalBuffer.toString());
console.log("Processed Buffer:", result.data);
console.log("Decoded Checksum:", result.checksum.toString(16));

const decrypted = processBuffer(result.data, xorKey);
console.log("Decrypted String:", decrypted.data.toString("utf-8"));