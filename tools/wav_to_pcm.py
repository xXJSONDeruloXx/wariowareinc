#!/usr/bin/env python3
import sys
import wave
from pathlib import Path


def unsigned8_to_signed8(data: bytes) -> bytes:
    return bytes(((b - 128) & 0xFF) for b in data)


def main() -> int:
    if len(sys.argv) != 3:
        print("usage: wav_to_pcm.py <input.wav> <output.pcm>", file=sys.stderr)
        return 2

    src = Path(sys.argv[1])
    dst = Path(sys.argv[2])
    dst.parent.mkdir(parents=True, exist_ok=True)

    with wave.open(str(src), "rb") as wav:
        channels = wav.getnchannels()
        sample_width = wav.getsampwidth()
        frames = wav.readframes(wav.getnframes())

    if channels != 1:
        raise ValueError(f"{src}: expected mono WAV, got {channels} channels")
    if sample_width != 1:
        raise ValueError(f"{src}: expected 8-bit WAV, got {sample_width * 8}-bit samples")

    dst.write_bytes(unsigned8_to_signed8(frames))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
