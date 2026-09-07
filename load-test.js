import http from 'k6/http';
import { check, sleep } from 'k6';

export const options = {
  stages: [
    { duration: '30s', target: 50 },  // עלייה הדרגתית ל-50 משתמשים במקביל
    { duration: '1m', target: 100 }, // החזקת עומס של 100 משתמשים למשך דקה
    { duration: '30s', target: 0 },   // ירדה והרגעת המערכת לסיום
  ],
};

export default function () {
  const res = http.get('http://localhost:8080/');
  check(res, {
    'status is 200': (r) => r.status === 200,
  });
  sleep(0.1);
}
