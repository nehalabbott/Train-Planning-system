import { useState } from "react";

const stations = [
  "Rajiv Chowk",
  "New Delhi",
  "Kashmere Gate",
  "Yamuna Bank",
  "Botanical Garden",
  "Noida Sector 62",
  "Moti Nagar",
  "Janakpuri West",
  "Dwarka Sector 21",
  "Hauz Khas",
  "AIIMS",
  "INA",
  "Green Park",
  "Saket",
  "Lajpat Nagar",
  "Mandi House",
  "Kalkaji Mandir",
  "Karol Bagh",
  "Chandni Chowk",
  "Central Secretariat"
];

export default function App() {
  const [source, setSource] = useState("");
  const [destination, setDestination] = useState("");
  const [routeType, setRouteType] = useState("distance");

  const [routeData, setRouteData] = useState(null);

  const handleFindRoute = () => {
    if (!source || !destination) {
      alert("Please select source and destination stations.");
      return;
    }

    // Dummy data for now
    setRouteData({
      path: [source, "Rajiv Chowk", destination],
      distance: 22,
      time: 28,
      fare: 30,
      interchanges: 1
    });
  };

  return (
    <div style={styles.page}>
      <div style={styles.card}>
        <h1 style={styles.title}>
          🚇 Delhi Metro Route Planner
        </h1>

        <div style={styles.inputGroup}>
          <label>Source Station</label>

          <select
            value={source}
            onChange={(e) =>
              setSource(e.target.value)
            }
          >
            <option value="">
              Select Source
            </option>

            {stations.map((station) => (
              <option
                key={station}
                value={station}
              >
                {station}
              </option>
            ))}
          </select>
        </div>

        <div style={styles.inputGroup}>
          <label>Destination Station</label>

          <select
            value={destination}
            onChange={(e) =>
              setDestination(e.target.value)
            }
          >
            <option value="">
              Select Destination
            </option>

            {stations.map((station) => (
              <option
                key={station}
                value={station}
              >
                {station}
              </option>
            ))}
          </select>
        </div>

        <div style={styles.inputGroup}>
          <label>Optimization</label>

          <select
            value={routeType}
            onChange={(e) =>
              setRouteType(e.target.value)
            }
          >
            <option value="distance">
              Shortest Distance
            </option>

            <option value="time">
              Fastest Route
            </option>

            <option value="interchange">
              Minimum Interchanges
            </option>
          </select>
        </div>

        <button
          style={styles.button}
          onClick={handleFindRoute}
        >
          Find Route
        </button>

        {routeData && (
          <div style={styles.result}>
            <h2>Route Details</h2>

            <div style={styles.pathContainer}>
              {routeData.path.map(
                (station, index) => (
                  <div key={index}>
                    <div
                      style={
                        styles.station
                      }
                    >
                      {station}
                    </div>

                    {index !==
                      routeData.path.length -
                        1 && (
                      <div
                        style={
                          styles.arrow
                        }
                      >
                        ↓
                      </div>
                    )}
                  </div>
                )
              )}
            </div>

            <div style={styles.stats}>
              <p>
                <strong>
                  Distance:
                </strong>{" "}
                {
                  routeData.distance
                }{" "}
                km
              </p>

              <p>
                <strong>
                  Time:
                </strong>{" "}
                {routeData.time} min
              </p>

              <p>
                <strong>
                  Fare:
                </strong>{" "}
                ₹{routeData.fare}
              </p>

              <p>
                <strong>
                  Interchanges:
                </strong>{" "}
                {
                  routeData.interchanges
                }
              </p>

              <p>
                <strong>
                  Optimization:
                </strong>{" "}
                {routeType}
              </p>
            </div>
          </div>
        )}
      </div>
    </div>
  );
}

const styles = {
  page: {
    minHeight: "100vh",
    background: "#f3f4f6",
    display: "flex",
    justifyContent: "center",
    alignItems: "center",
    padding: "40px"
  },

  card: {
    background: "white",
    width: "700px",
    padding: "30px",
    borderRadius: "16px",
    boxShadow:
      "0 10px 30px rgba(0,0,0,0.1)"
  },

  title: {
    textAlign: "center",
    marginBottom: "30px"
  },

  inputGroup: {
    display: "flex",
    flexDirection: "column",
    marginBottom: "20px"
  },

  button: {
    width: "100%",
    padding: "14px",
    background: "#2563eb",
    color: "white",
    border: "none",
    borderRadius: "8px",
    fontSize: "16px",
    cursor: "pointer"
  },

  result: {
    marginTop: "30px",
    paddingTop: "20px",
    borderTop: "1px solid #ddd"
  },

  pathContainer: {
    marginTop: "15px",
    marginBottom: "20px"
  },

  station: {
    fontWeight: "bold",
    padding: "6px"
  },

  arrow: {
    textAlign: "center",
    fontSize: "22px",
    color: "#2563eb"
  },

  stats: {
    background: "#f8fafc",
    padding: "15px",
    borderRadius: "10px"
  }
};