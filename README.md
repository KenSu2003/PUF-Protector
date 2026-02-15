# PUF-Tester

**Test and characterization scripts** for a PUF (Physically Unclonable Function) implemented on a **Xilinx Artix FPGA**. This repo contains the host-side scripts only—the PUF design itself runs on the FPGA. The scripts challenge the PUF over **SPI** (via an FTDI USB–SPI bridge) and compute **reliability** and **uniqueness** metrics from the response data. The PUF is challenged over **SPI** (via an FTDI USB–SPI bridge), and the response data is used to compute **reliability** and **uniqueness** metrics.

---

## Purpose

- **Automate PUF readout:** Send challenges to the PUF on the Artix over SPI and record responses.
- **Evaluate PUF quality:** Compute reliability (response stability across multiple reads) and uniqueness (difference between responses from different devices / runs).

---

## Setup

- **Hardware:** Xilinx Artix FPGA with the PUF design; **FTDI FT2232H** (or compatible) USB‑to‑SPI adapter connected to the board.
- **Host:** Python 3; `pyftdi` for SPI communication.

Install dependencies:

```bash
pip install pyftdi
```

Connect the FTDI device (e.g. `ftdi://ftdi:2232h/1` as in the script). Ensure the FPGA is programmed and the SPI interface matches the protocol (command nibble + data) used in the code.

---

## Project Structure

| File | Role |
|------|------|
| **automated_script.py** | Talks to the PUF over SPI via PyFTDI. Configures timing (delta-high / delta-rest in clock cycles) and number of runs, starts a run, reads 16-byte responses per challenge, and writes them to `responses.csv`. |
| **SPI_Interface.py** | PUF metrics: Hamming distance, **reliability** (vs a reference/golden response), **uniqueness** (pairwise Hamming distance across responses), and helper to compute a golden response (mode of a set of responses). |
| **testing.py** | Loads `responses.csv`, uses `SPI_Interface` to compute reliability and uniqueness, and prints the results. |

---

## Protocol (SPI)

The script uses a simple command protocol over SPI:

- **Lower 4 bits:** Command (`START_CMD`, `SET_RUNS_CMD`, `SET_DELTA_HIGH`, `SET_DELTA_REST`).
- **Upper 4 bits:** Data (e.g. number of runs, timing values).

After configuring timing and run count, a start command triggers the PUF; the host reads back `16 × k` bytes (16 bytes per run).

---

## Workflow

1. Program the Artix with the PUF design and connect the FTDI SPI adapter.
2. Run **automated_script.py** to challenge the PUF and generate **responses.csv**.
3. Run **testing.py** to load the responses and print **reliability** and **uniqueness** (and optionally use the same metrics on other response sets for comparison).

---

## Output

- **responses.csv:** Raw PUF response bytes (e.g. 16 bytes per row per run), produced by `automated_script.py`.
- **Console:** Reliability and uniqueness percentages from `testing.py`.
